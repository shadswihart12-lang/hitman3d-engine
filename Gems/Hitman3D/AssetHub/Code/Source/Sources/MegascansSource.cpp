// ═══════════════════════════════════════════════════════════════
// MegascansSource.cpp — STUB
// Returns "not implemented" for all calls. Wire up post-Epic-auth.
// ═══════════════════════════════════════════════════════════════

#include "MegascansSource.h"

namespace Hitman3D::AssetHub
{
    void MegascansSource::Authenticate(AuthCallback onDone)
    {
        AuthResult r;
        r.success = false;
        r.errorMessage = "Megascans source not yet implemented. Epic auth flow pending.";
        if (onDone) onDone(r);
    }

    RequestId MegascansSource::Search(const SearchQuery& /*query*/, SearchCallback onDone)
    {
        SearchResult r;
        r.success = false;
        r.errorMessage = "Megascans search not yet implemented.";
        if (onDone) onDone(r);
        return 0;
    }

    RequestId MegascansSource::Download(const AZStd::string& /*assetId*/, DownloadCallback onDone)
    {
        DownloadResult r;
        r.success = false;
        r.errorMessage = "Megascans download not yet implemented.";
        if (onDone) onDone(r);
        return 0;
    }
}
