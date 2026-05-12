// ═══════════════════════════════════════════════════════════════
// IAssetSource.h
// Abstract interface every marketplace adapter implements.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/Memory/Memory.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/functional.h>

#include "AssetMetadata.h"

namespace Hitman3D::AssetHub
{
    using RequestId = AZ::u64;

    //! Result type for search calls.
    struct SearchResult
    {
        bool success = false;
        AZStd::string errorMessage;
        AZStd::vector<AssetMetadata> assets;
        AZ::u64 totalAvailable = 0;   // server-reported total (for paging)
    };

    //! Result type for downloads.
    struct DownloadResult
    {
        bool success = false;
        AZStd::string errorMessage;
        AZStd::string localFilePath;  // absolute path to downloaded file (zip / fbx / glb)
        AssetMetadata metadata;       // echoed back so importer has full context
    };

    //! Auth state after Authenticate() completes.
    struct AuthResult
    {
        bool success = false;
        AZStd::string errorMessage;
        AZStd::string accountIdentifier;  // username / email for display only
    };

    using SearchCallback   = AZStd::function<void(const SearchResult&)>;
    using DownloadCallback = AZStd::function<void(const DownloadResult&)>;
    using AuthCallback     = AZStd::function<void(const AuthResult&)>;

    //! Abstract marketplace adapter. One concrete implementation per source.
    class IAssetSource
    {
    public:
        AZ_RTTI(IAssetSource, "{8e2a1f5c-1234-4abc-9def-0123456789ab}");

        virtual ~IAssetSource() = default;

        // ─── Identity ─────────────────────────────────────────
        //! Stable machine ID — used in metadata, filesystem paths, settings keys.
        virtual const char* GetSourceId() const = 0;
        //! Human-readable name for UI.
        virtual const char* GetDisplayName() const = 0;
        //! Returns false until Authenticate() succeeds (or N/A for anonymous sources).
        virtual bool IsReady() const = 0;
        //! True if the source has been wired up (some are stubs until Epic approves Fab).
        virtual bool IsImplemented() const = 0;

        // ─── Auth ─────────────────────────────────────────────
        //! Kick off async auth. Result delivered via callback on main thread.
        //! For anonymous sources, immediately invokes callback with success=true.
        virtual void Authenticate(AuthCallback onDone) = 0;
        //! Clear stored credentials. Always succeeds.
        virtual void Logout() = 0;

        // ─── Operations ───────────────────────────────────────
        //! Search the source's catalog. Returns request ID for cancellation.
        virtual RequestId Search(const SearchQuery& query, SearchCallback onDone) = 0;
        //! Download a specific asset by ID. Callback gives local file path.
        virtual RequestId Download(const AZStd::string& assetId, DownloadCallback onDone) = 0;
        //! Cancel any in-flight request. Safe to call with stale IDs.
        virtual void Cancel(RequestId requestId) = 0;
    };
}
