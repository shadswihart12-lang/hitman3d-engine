// ═══════════════════════════════════════════════════════════════
// FabSource.cpp — Placeholder until Epic approves Fab Developer API
// ═══════════════════════════════════════════════════════════════

#include "FabSource.h"

namespace Hitman3D::AssetHub
{
    void FabSource::Authenticate(AuthCallback onDone)
    {
        AuthResult r;
        r.success = false;
        r.errorMessage = "Fab integration pending Epic Developer API approval. "
                         "Use manual import: download from fab.com, drop into "
                         "<Project>/Assets/Imported/Fab_Inbox/, plugin will auto-import.";
        if (onDone) onDone(r);
    }

    RequestId FabSource::Search(const SearchQuery& /*query*/, SearchCallback onDone)
    {
        SearchResult r;
        r.success = false;
        r.errorMessage = "Fab API access pending Epic approval.";
        if (onDone) onDone(r);
        return 0;
    }

    RequestId FabSource::Download(const AZStd::string& /*assetId*/, DownloadCallback onDone)
    {
        DownloadResult r;
        r.success = false;
        r.errorMessage = "Fab API access pending Epic approval.";
        if (onDone) onDone(r);
        return 0;
    }
}
