// ═══════════════════════════════════════════════════════════════
// AssetHubSystemComponent.h
// The main runtime component — registers all source adapters,
// routes EBus calls, owns the request lifecycle.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/containers/unordered_map.h>

#include <Hitman3D/AssetHub/AssetHubBus.h>

namespace Hitman3D::AssetHub
{
    class AssetHubSystemComponent
        : public AZ::Component
        , protected AssetHubBus::Handler
    {
    public:
        AZ_COMPONENT(AssetHubSystemComponent, "{55667788-9999-aaaa-bbbb-ccccddddeeff}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        AssetHubSystemComponent();
        ~AssetHubSystemComponent() override;

    protected:
        // AZ::Component ───────────────────────────────────
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        // AssetHubBus::Handler ────────────────────────────
        AZStd::vector<AZStd::string> GetSourceIds() const override;
        IAssetSource* GetSource(const AZStd::string& sourceId) override;

        RequestId SearchSource(
            const AZStd::string& sourceId,
            const SearchQuery& query,
            SearchCallback onDone) override;

        RequestId QueueImport(
            const AZStd::string& sourceId,
            const AZStd::string& assetId,
            const AZStd::string& targetProjectRelPath,
            AZStd::function<void(bool, const AZStd::string&, const AZStd::string&)> onDone) override;

    private:
        void RegisterSources();

        //! Owned source adapters. Map for stable string-key lookup.
        AZStd::unordered_map<AZStd::string, AZStd::unique_ptr<IAssetSource>> m_sources;
    };
}
