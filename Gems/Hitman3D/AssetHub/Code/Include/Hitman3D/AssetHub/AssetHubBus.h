// ═══════════════════════════════════════════════════════════════
// AssetHubBus.h
// EBus interface for editor/game code to talk to the Asset Hub system.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include "IAssetSource.h"

namespace Hitman3D::AssetHub
{
    //! High-level operations exposed by AssetHubSystemComponent.
    //! Called from UI panel, editor commands, automation scripts.
    class AssetHubRequests
    {
    public:
        AZ_RTTI(AssetHubRequests, "{a0a1a2a3-4444-5555-6666-aabbccddeeff}");
        virtual ~AssetHubRequests() = default;

        //! Enumerate all registered source IDs.
        virtual AZStd::vector<AZStd::string> GetSourceIds() const = 0;
        //! Get pointer to a source by ID. Returns nullptr if unknown.
        virtual IAssetSource* GetSource(const AZStd::string& sourceId) = 0;

        //! Convenience: search a specific source.
        virtual RequestId SearchSource(
            const AZStd::string& sourceId,
            const SearchQuery& query,
            SearchCallback onDone) = 0;

        //! Convenience: one-shot import. Downloads + runs import pipeline.
        //! `targetProjectRelPath` is where in the project to place the asset,
        //! e.g. "Assets/Imported/Sketchfab/viking_helmet_a3f9/".
        //! Callback fires with success/failure when import completes.
        virtual RequestId QueueImport(
            const AZStd::string& sourceId,
            const AZStd::string& assetId,
            const AZStd::string& targetProjectRelPath,
            AZStd::function<void(bool success, const AZStd::string& finalPath, const AZStd::string& error)> onDone) = 0;
    };

    //! EBus traits — single bus, single handler (the system component).
    class AssetHubBusTraits : public AZ::EBusTraits
    {
    public:
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
    };

    using AssetHubBus = AZ::EBus<AssetHubRequests, AssetHubBusTraits>;

    //! Notifications fired by the system as imports progress.
    class AssetHubNotifications
    {
    public:
        AZ_RTTI(AssetHubNotifications, "{b1b2b3b4-5555-6666-7777-bbccddeeff00}");
        virtual ~AssetHubNotifications() = default;

        virtual void OnImportStarted(const AssetMetadata& /*meta*/) {}
        virtual void OnImportProgress(const AZStd::string& /*assetId*/, float /*pct*/) {}
        virtual void OnImportCompleted(const AssetMetadata& /*meta*/, const AZStd::string& /*localPath*/) {}
        virtual void OnImportFailed(const AssetMetadata& /*meta*/, const AZStd::string& /*error*/) {}
    };

    using AssetHubNotificationBus = AZ::EBus<AssetHubNotifications, AssetHubBusTraits>;
}
