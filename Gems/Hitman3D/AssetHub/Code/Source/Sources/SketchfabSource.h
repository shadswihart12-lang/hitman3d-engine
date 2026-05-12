// ═══════════════════════════════════════════════════════════════
// SketchfabSource.h
// Sketchfab adapter — public REST API at api.sketchfab.com/v3.
// OAuth2 device-flow auth. Downloads as GLB for static meshes.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <Hitman3D/AssetHub/IAssetSource.h>
#include <AzCore/std/parallel/mutex.h>
#include <AzCore/std/containers/unordered_map.h>

namespace Hitman3D::AssetHub
{
    class SketchfabSource final : public IAssetSource
    {
    public:
        AZ_RTTI(SketchfabSource, "{12345678-aaaa-bbbb-cccc-1234567890ab}", IAssetSource);
        AZ_CLASS_ALLOCATOR(SketchfabSource, AZ::SystemAllocator);

        SketchfabSource();
        ~SketchfabSource() override;

        // IAssetSource ───────────────────────────────────────────
        const char* GetSourceId() const override { return "sketchfab"; }
        const char* GetDisplayName() const override { return "Sketchfab"; }
        bool IsReady() const override;
        bool IsImplemented() const override { return true; }

        void Authenticate(AuthCallback onDone) override;
        void Logout() override;

        RequestId Search(const SearchQuery& query, SearchCallback onDone) override;
        RequestId Download(const AZStd::string& assetId, DownloadCallback onDone) override;
        void Cancel(RequestId requestId) override;

    private:
        // OAuth2 details — our registered Sketchfab app credentials.
        // CLIENT_ID is studio-bound, registered at https://sketchfab.com/developers/oauth
        // CLIENT_SECRET stays in Windows Credential Manager, NEVER in source.
        static constexpr const char* kApiBase = "https://api.sketchfab.com/v3";
        static constexpr const char* kClientId = "REPLACE_WITH_REGISTERED_CLIENT_ID";

        //! Internal: convert Sketchfab license slug → our LicenseClass.
        static LicenseClass ParseLicense(const AZStd::string& slug);
        //! Internal: parse a search-response JSON document into AssetMetadata list.
        static SearchResult ParseSearchResponse(const AZStd::string& jsonBody);

        //! Stored bearer token (in-memory cache; persistent copy lives in CredentialStore).
        AZStd::string m_bearerToken;
        bool m_isReady = false;

        //! Request tracking for cancellation.
        AZStd::mutex m_requestMutex;
        AZStd::unordered_map<RequestId, AZStd::string> m_inFlightRequests; // id → http handle
        AZ::u64 m_nextRequestId = 1;
    };
}
