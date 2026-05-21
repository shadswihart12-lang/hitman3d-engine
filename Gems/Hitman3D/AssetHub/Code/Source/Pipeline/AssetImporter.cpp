// ═══════════════════════════════════════════════════════════════
// AssetImporter.cpp
// Implementation of the asset-import pipeline.
// ═══════════════════════════════════════════════════════════════

#include "AssetImporter.h"

#include <AzCore/IO/FileIO.h>
#include <AzCore/IO/SystemFile.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/JSON/document.h>
#include <AzCore/JSON/prettywriter.h>
#include <AzCore/JSON/stringbuffer.h>
#include <AzCore/Jobs/JobFunction.h>
#include <AzCore/StringFunc/StringFunc.h>
#include <AzCore/Settings/SettingsRegistry.h>

#include <Hitman3D/AssetHub/AssetHubHttpClient.h>

namespace Hitman3D::AssetHub
{
    namespace
    {
        const char* LicenseClassToString(LicenseClass c)
        {
            switch (c)
            {
                case LicenseClass::CC0:         return "CC0";
                case LicenseClass::CC_BY:       return "CC-BY";
                case LicenseClass::CC_BY_SA:    return "CC-BY-SA";
                case LicenseClass::CC_BY_NC:    return "CC-BY-NC";
                case LicenseClass::Proprietary: return "Proprietary";
                case LicenseClass::Custom:      return "Custom";
                default:                        return "Unknown";
            }
        }

        const char* AssetCategoryToString(AssetCategory c)
        {
            switch (c)
            {
                case AssetCategory::StaticMesh:   return "StaticMesh";
                case AssetCategory::SkeletalMesh: return "SkeletalMesh";
                case AssetCategory::Material:     return "Material";
                case AssetCategory::Texture:      return "Texture";
                case AssetCategory::HDRI:         return "HDRI";
                case AssetCategory::Audio:        return "Audio";
                case AssetCategory::Animation:    return "Animation";
                default:                          return "Unknown";
            }
        }

        bool IsArchive(const AZStd::string& path)
        {
            return AZ::StringFunc::EndsWith(path, ".zip", false /*caseSensitive*/)
                || AZ::StringFunc::EndsWith(path, ".tar.gz", false)
                || AZ::StringFunc::EndsWith(path, ".tgz", false);
        }
    }

    void AssetImporter::Import(const ImportRequest& request, ImportCallback onDone)
    {
        // Run on a job so we don't block the calling (main) thread.
        AZ::JobContext* jc = nullptr;
        auto* job = AZ::CreateJobFunction(
            [request, onDone]()
            {
                ImportResult result;

                // Step 1: ensure we have a local file
                AZStd::string localPath;
                AZStd::string err;
                if (!FetchToLocal(request.sourceLocalPath, localPath, err))
                {
                    result.errorMessage = err;
                    if (onDone) onDone(result);
                    return;
                }

                // Make sure the target dir exists
                AZ::IO::FileIOBase* io = AZ::IO::FileIOBase::GetInstance();
                if (io)
                {
                    io->CreatePath(request.targetProjectPath.c_str());
                }

                // Step 2: unpack if archive, pick primary mesh
                AZStd::string primaryMesh = localPath;
                if (IsArchive(localPath))
                {
                    if (!Unpack(localPath, request.targetProjectPath, primaryMesh, err))
                    {
                        result.errorMessage = err;
                        if (onDone) onDone(result);
                        return;
                    }
                }

                // Step 3: process mesh through SceneAPI
                AZStd::string finalAsset;
                if (!ProcessMesh(primaryMesh, request.targetProjectPath, request, finalAsset, err))
                {
                    result.errorMessage = err;
                    if (onDone) onDone(result);
                    return;
                }

                // Step 4: write attribution sidecar
                if (!WriteMetadata(request.metadata, request.targetProjectPath))
                {
                    // Non-fatal — log but don't fail the import.
                    AZ_Warning("Hitman3D::AssetHub", false,
                        "Failed to write .h3d_meta.json sidecar — attribution will be missing");
                }

                result.success = true;
                result.finalAssetPath = finalAsset;
                if (onDone) onDone(result);
            },
            true /*isAutoDelete*/, jc);
        job->Start();
    }

    bool AssetImporter::FetchToLocal(const AZStd::string& urlOrPath, AZStd::string& outLocalPath, AZStd::string& outError)
    {
        // If it starts with http(s), download. Otherwise treat as already-local.
        if (!AZ::StringFunc::StartsWith(urlOrPath, "http://", false /*caseSensitive*/) &&
            !AZ::StringFunc::StartsWith(urlOrPath, "https://", false))
        {
            outLocalPath = urlOrPath;
            return true;
        }

        AZStd::string body;
        HttpResponseCode code = HttpResponseCode::RequestNotMade;
        if (!AssetHubHttpClient::GetBlocking(urlOrPath, {}, body, code, outError))
        {
            return false;
        }

        if (code != HttpResponseCode::Ok)
        {
            outError = AZStd::string::format("HTTP fetch failed: %d", static_cast<int>(code));
            return false;
        }

        // Stage to a temp file in user-cache.
        AZ::IO::Path cacheDir;
        if (auto* settings = AZ::SettingsRegistry::Get())
        {
            AZ::IO::FixedMaxPath fp;
            settings->Get(fp.Native(), AZ::SettingsRegistryMergeUtils::FilePathKey_CacheUserPath);
            cacheDir = fp;
        }
        cacheDir /= "AssetHub_Downloads";

        AZ::IO::FileIOBase* io = AZ::IO::FileIOBase::GetInstance();
        if (io) io->CreatePath(cacheDir.c_str());

        // Use a UID-based filename — we don't know the extension yet, but most downloads are .glb or .zip.
        // SniffFor exteinson from first few bytes in production.
        AZ::IO::Path filePath = cacheDir / "download.bin";

        AZ::IO::SystemFile sysFile;
        if (!sysFile.Open(filePath.c_str(),
            AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY))
        {
            outError = "Failed to open cache file for write";
            return false;
        }
        sysFile.Write(body.data(), body.size());
        sysFile.Close();

        outLocalPath = filePath.String();
        return true;
    }

    bool AssetImporter::Unpack(const AZStd::string& localPath, const AZStd::string& destDir,
                                AZStd::string& outPrimaryMesh, AZStd::string& outError)
    {
        // TODO: integrate libzip or minizip — bundled with O3DE 3rdparty.
        // For v1 stub, assume the file is already a usable mesh.
        (void)destDir;
        outPrimaryMesh = localPath;
        outError.clear();
        return true;
    }

    bool AssetImporter::ProcessMesh(const AZStd::string& meshFile, const AZStd::string& targetDir,
                                    const ImportRequest& req, AZStd::string& outFinalAsset, AZStd::string& outError)
    {
        // O3DE's AssetProcessor watches the project's Assets folder and converts FBX/GLB/OBJ
        // automatically. So step 3 is "copy the file into the target dir and let AP do its thing."
        //
        // For non-trivial pipelines (LODs, collision overrides, custom material remap) we'd emit
        // a .scenesettings sidecar that drives AZ::SceneAPI's import.

        AZ::IO::FileIOBase* io = AZ::IO::FileIOBase::GetInstance();
        if (!io)
        {
            outError = "FileIOBase unavailable";
            return false;
        }

        AZ::IO::Path destFile = AZ::IO::Path(targetDir) / AZ::IO::PathView(meshFile).Filename();

        if (!io->Copy(meshFile.c_str(), destFile.c_str()))
        {
            outError = AZStd::string::format("Copy to project failed: %s → %s",
                meshFile.c_str(), destFile.c_str());
            return false;
        }

        // Emit a basic .scenesettings if user wants collision/LODs.
        if (req.generateCollision || req.generateLODs)
        {
            AZ::IO::Path settingsPath = destFile;
            settingsPath += ".scenesettings";

            rapidjson::Document settings(rapidjson::kObjectType);
            auto& alloc = settings.GetAllocator();

            if (req.generateCollision)
            {
                rapidjson::Value physCfg(rapidjson::kObjectType);
                physCfg.AddMember("collisionType", "convexHull", alloc);
                settings.AddMember("physics", physCfg, alloc);
            }
            if (req.generateLODs)
            {
                rapidjson::Value lodCfg(rapidjson::kObjectType);
                lodCfg.AddMember("autoGenerate", true, alloc);
                lodCfg.AddMember("levels", 3, alloc);
                settings.AddMember("lods", lodCfg, alloc);
            }

            rapidjson::StringBuffer buf;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf);
            settings.Accept(writer);

            AZ::IO::SystemFile sf;
            if (sf.Open(settingsPath.c_str(),
                AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY))
            {
                sf.Write(buf.GetString(), buf.GetSize());
                sf.Close();
            }
        }

        outFinalAsset = destFile.String();
        outError.clear();
        return true;
    }

    bool AssetImporter::WriteMetadata(const AssetMetadata& meta, const AZStd::string& targetDir)
    {
        rapidjson::Document doc(rapidjson::kObjectType);
        auto& alloc = doc.GetAllocator();

        auto addStr = [&](const char* key, const AZStd::string& v)
        {
            doc.AddMember(rapidjson::StringRef(key),
                rapidjson::Value(v.c_str(), static_cast<rapidjson::SizeType>(v.size()), alloc),
                alloc);
        };

        addStr("source", meta.sourceId);
        addStr("assetId", meta.assetId);
        addStr("displayName", meta.displayName);
        addStr("author", meta.author);
        addStr("originalUrl", meta.originalUrl);
        addStr("license", LicenseClassToString(meta.license));
        addStr("licenseText", meta.licenseText);
        addStr("category", AssetCategoryToString(meta.category));

        doc.AddMember("polyCount", meta.polyCount, alloc);
        doc.AddMember("fileSizeBytes", meta.fileSizeBytes, alloc);

        rapidjson::Value tags(rapidjson::kArrayType);
        for (const auto& t : meta.tags)
        {
            tags.PushBack(rapidjson::Value(t.c_str(), static_cast<rapidjson::SizeType>(t.size()), alloc), alloc);
        }
        doc.AddMember("tags", tags, alloc);

        rapidjson::StringBuffer buf;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf);
        doc.Accept(writer);

        AZ::IO::Path sidecarPath = AZ::IO::Path(targetDir) / ".h3d_meta.json";
        AZ::IO::SystemFile sf;
        if (!sf.Open(sidecarPath.c_str(),
            AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY))
        {
            return false;
        }
        sf.Write(buf.GetString(), buf.GetSize());
        sf.Close();
        return true;
    }
}
