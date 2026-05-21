// ═══════════════════════════════════════════════════════════════
// AssetHubSystemComponent.cpp
// ═══════════════════════════════════════════════════════════════

#include "AssetHubSystemComponent.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#include "Sources/SketchfabSource.h"
#include "Sources/MegascansSource.h"
#include "Sources/OpenGameArtSource.h"
#include "Sources/FabSource.h"

#include "Pipeline/AssetImporter.h"

namespace Hitman3D::AssetHub
{
    AssetHubSystemComponent::AssetHubSystemComponent() = default;
    AssetHubSystemComponent::~AssetHubSystemComponent() = default;

    void AssetHubSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<AssetHubSystemComponent, AZ::Component>()
                ->Version(1);

            if (auto* ec = sc->GetEditContext())
            {
                ec->Class<AssetHubSystemComponent>("Hitman 3d Asset Hub",
                    "One-click asset import from Sketchfab, Megascans, OpenGameArt, Fab")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Hitman 3d")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu,
                        AZ_CRC_CE("System"));
            }
        }
    }

    void AssetHubSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Hitman3DAssetHubService"));
    }
    void AssetHubSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Hitman3DAssetHubService"));
    }
    void AssetHubSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& /*required*/)
    {
    }

    void AssetHubSystemComponent::Init()
    {
        RegisterSources();
    }

    void AssetHubSystemComponent::Activate()
    {
        AssetHubBus::Handler::BusConnect();
    }

    void AssetHubSystemComponent::Deactivate()
    {
        AssetHubBus::Handler::BusDisconnect();
    }

    void AssetHubSystemComponent::RegisterSources()
    {
        m_sources["sketchfab"]    = AZStd::make_unique<SketchfabSource>();
        m_sources["megascans"]    = AZStd::make_unique<MegascansSource>();
        m_sources["openGameArt"]  = AZStd::make_unique<OpenGameArtSource>();
        m_sources["fab"]          = AZStd::make_unique<FabSource>();
    }

    AZStd::vector<AZStd::string> AssetHubSystemComponent::GetSourceIds() const
    {
        AZStd::vector<AZStd::string> out;
        out.reserve(m_sources.size());
        for (const auto& [k, _] : m_sources) out.push_back(k);
        return out;
    }

    IAssetSource* AssetHubSystemComponent::GetSource(const AZStd::string& sourceId)
    {
        auto it = m_sources.find(sourceId);
        return (it != m_sources.end()) ? it->second.get() : nullptr;
    }

    RequestId AssetHubSystemComponent::SearchSource(
        const AZStd::string& sourceId,
        const SearchQuery& query,
        SearchCallback onDone)
    {
        IAssetSource* src = GetSource(sourceId);
        if (!src)
        {
            SearchResult err;
            err.success = false;
            err.errorMessage = AZStd::string::format("Unknown source: %s", sourceId.c_str());
            if (onDone) onDone(err);
            return 0;
        }
        if (!src->IsImplemented())
        {
            SearchResult err;
            err.success = false;
            err.errorMessage = AZStd::string::format("Source '%s' not yet implemented", sourceId.c_str());
            if (onDone) onDone(err);
            return 0;
        }
        return src->Search(query, AZStd::move(onDone));
    }

    RequestId AssetHubSystemComponent::QueueImport(
        const AZStd::string& sourceId,
        const AZStd::string& assetId,
        const AZStd::string& targetProjectRelPath,
        AZStd::function<void(bool, const AZStd::string&, const AZStd::string&)> onDone)
    {
        IAssetSource* src = GetSource(sourceId);
        if (!src || !src->IsImplemented())
        {
            if (onDone) onDone(false, {}, AZStd::string::format("Source '%s' unavailable", sourceId.c_str()));
            return 0;
        }

        // Fire notification: import started
        AssetHubNotificationBus::Broadcast(
            &AssetHubNotifications::OnImportStarted, AssetMetadata{});  // metadata filled in after download

        return src->Download(assetId,
            [targetProjectRelPath, sourceId, assetId, onDone = AZStd::move(onDone)]
            (const DownloadResult& dl)
            {
                if (!dl.success)
                {
                    AssetHubNotificationBus::Broadcast(
                        &AssetHubNotifications::OnImportFailed, dl.metadata, dl.errorMessage);
                    if (onDone) onDone(false, {}, dl.errorMessage);
                    return;
                }

                ImportRequest req;
                req.sourceLocalPath    = dl.localFilePath;
                req.targetProjectPath  = targetProjectRelPath;
                req.metadata           = dl.metadata;
                req.metadata.sourceId  = sourceId;
                req.metadata.assetId   = assetId;
                req.generateCollision  = true;
                req.generateLODs       = true;

                AssetImporter::Import(req,
                    [meta = req.metadata, onDone](const ImportResult& ir)
                    {
                        if (ir.success)
                        {
                            AssetHubNotificationBus::Broadcast(
                                &AssetHubNotifications::OnImportCompleted, meta, ir.finalAssetPath);
                            if (onDone) onDone(true, ir.finalAssetPath, AZStd::string{});
                        }
                        else
                        {
                            AssetHubNotificationBus::Broadcast(
                                &AssetHubNotifications::OnImportFailed, meta, ir.errorMessage);
                            if (onDone) onDone(false, {}, ir.errorMessage);
                        }
                    });
            });
    }
}
