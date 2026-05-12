// ═══════════════════════════════════════════════════════════════
// OpenGameArtSource.h — OpenGameArt.org adapter (STUB)
// Part of: Hitman 3d Asset Hub
//
// Status: Stub. OGA has no formal API — implementation will scrape
// their RSS feeds and search-results JSON. Respect their rate limit
// (max 1 req/sec) and User-Agent guidelines.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <Hitman3D/AssetHub/IAssetSource.h>

namespace Hitman3D::AssetHub
{
    class OpenGameArtSource final : public IAssetSource
    {
    public:
        AZ_RTTI(OpenGameArtSource, "{def56789-aaaa-bbbb-cccc-ddddeeeeffff}", IAssetSource);
        AZ_CLASS_ALLOCATOR(OpenGameArtSource, AZ::SystemAllocator);

        const char* GetSourceId() const override { return "openGameArt"; }
        const char* GetDisplayName() const override { return "OpenGameArt"; }
        bool IsReady() const override { return true; }   // anonymous source — always "ready"
        bool IsImplemented() const override { return false; }

        void Authenticate(AuthCallback onDone) override
        {
            AuthResult r; r.success = true; r.accountIdentifier = "anonymous";
            if (onDone) onDone(r);
        }
        void Logout() override {}

        RequestId Search(const SearchQuery& query, SearchCallback onDone) override;
        RequestId Download(const AZStd::string& assetId, DownloadCallback onDone) override;
        void Cancel(RequestId /*requestId*/) override {}
    };
}
