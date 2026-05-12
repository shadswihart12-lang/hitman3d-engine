// ═══════════════════════════════════════════════════════════════
// FabSource.h — Epic Fab adapter (PLACEHOLDER)
// Part of: Hitman 3d Asset Hub
//
// Status: Placeholder. Activates when Epic approves our Fab Developer
// API application. Until then, IsImplemented() returns false and UI
// surfaces a "Manual import: download from fab.com, drop in inbox"
// fallback path.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <Hitman3D/AssetHub/IAssetSource.h>

namespace Hitman3D::AssetHub
{
    class FabSource final : public IAssetSource
    {
    public:
        AZ_RTTI(FabSource, "{f4b00000-0000-4abc-8def-fab000000fab}", IAssetSource);
        AZ_CLASS_ALLOCATOR(FabSource, AZ::SystemAllocator);

        const char* GetSourceId() const override { return "fab"; }
        const char* GetDisplayName() const override { return "Fab (Epic)"; }
        bool IsReady() const override { return false; }
        bool IsImplemented() const override { return false; }

        void Authenticate(AuthCallback onDone) override;
        void Logout() override {}

        RequestId Search(const SearchQuery& query, SearchCallback onDone) override;
        RequestId Download(const AZStd::string& assetId, DownloadCallback onDone) override;
        void Cancel(RequestId /*requestId*/) override {}
    };
}
