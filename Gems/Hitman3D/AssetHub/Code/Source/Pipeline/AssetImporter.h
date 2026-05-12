// ═══════════════════════════════════════════════════════════════
// AssetImporter.h
// Takes a downloaded file (from any IAssetSource), unpacks if needed,
// runs it through O3DE's SceneAPI to produce engine-native assets,
// and writes a .h3d_meta.json attribution sidecar.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/std/string/string.h>
#include <AzCore/std/functional.h>
#include <Hitman3D/AssetHub/AssetMetadata.h>

namespace Hitman3D::AssetHub
{
    struct ImportRequest
    {
        AZStd::string sourceLocalPath;     // path or signed URL of downloaded payload
        AZStd::string targetProjectPath;   // project-relative dest, e.g. "Assets/Imported/Sketchfab/asset_id/"
        AssetMetadata metadata;            // for sidecar + attribution
        bool generateCollision = true;
        bool generateLODs = true;
        bool spawnInScene = false;         // drop into active editor scene at cursor
    };

    struct ImportResult
    {
        bool success = false;
        AZStd::string errorMessage;
        AZStd::string finalAssetPath;      // path to top-level imported asset (e.g. .azmodel)
    };

    using ImportCallback = AZStd::function<void(const ImportResult&)>;

    //! Static-only utility — stateless. Multiple imports can run in parallel.
    class AssetImporter
    {
    public:
        //! Run the full import pipeline. Async — callback fires on main thread.
        static void Import(const ImportRequest& request, ImportCallback onDone);

    private:
        //! Step 1 — fetch the file if sourceLocalPath is actually a URL.
        static bool FetchToLocal(const AZStd::string& urlOrPath, AZStd::string& outLocalPath, AZStd::string& outError);
        //! Step 2 — unpack ZIP/tar.gz if archive; return path to primary mesh file.
        static bool Unpack(const AZStd::string& localPath, const AZStd::string& destDir, AZStd::string& outPrimaryMesh, AZStd::string& outError);
        //! Step 3 — submit mesh file to SceneAPI for engine asset build.
        static bool ProcessMesh(const AZStd::string& meshFile, const AZStd::string& targetDir, const ImportRequest& req, AZStd::string& outFinalAsset, AZStd::string& outError);
        //! Step 4 — write .h3d_meta.json sidecar.
        static bool WriteMetadata(const AssetMetadata& meta, const AZStd::string& targetDir);
    };
}
