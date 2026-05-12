// ═══════════════════════════════════════════════════════════════
// MegascansSource.h — Quixel Megascans adapter (STUB)
// Part of: Hitman 3d Asset Hub
//
// Status: Stub. Implementation pending Epic auth flow finalization.
// Quixel API: https://quixel.com/megascans (auth via Epic account)
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <Hitman3D/AssetHub/IAssetSource.h>

namespace Hitman3D::AssetHub
{
    class MegascansSource final : public IAssetSource
    {
    public:
        AZ_RTTI(MegascansSource, "{abc12345-1111-2222-3333-444455556666}", IAssetSource);
        AZ_CLASS_ALLOCATOR(MegascansSource, AZ::SystemAllocator);

        const char* GetSourceId() const override { return "megascans"; }
        const char* GetDisplayName() const override { return "Quixel Megascans"; }
        bool IsReady() const override { return false; }
        bool IsImplemented() const override { return false; }

        void Authenticate(AuthCallback onDone) override;
        void Logout() override {}

        RequestId Search(const SearchQuery& query, SearchCallback onDone) override;
        RequestId Download(const AZStd::string& assetId, DownloadCallback onDone) override;
        void Cancel(RequestId /*requestId*/) override {}
    };
}
