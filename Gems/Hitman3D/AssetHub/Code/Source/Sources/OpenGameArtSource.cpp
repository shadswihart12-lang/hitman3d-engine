// ═══════════════════════════════════════════════════════════════
// OpenGameArtSource.cpp — STUB
// Returns "not implemented" for all calls.
// Future: scrape https://opengameart.org/art-search-advanced + RSS.
// ═══════════════════════════════════════════════════════════════

#include "OpenGameArtSource.h"

namespace Hitman3D::AssetHub
{
    RequestId OpenGameArtSource::Search(const SearchQuery& /*query*/, SearchCallback onDone)
    {
        SearchResult r;
        r.success = false;
        r.errorMessage = "OpenGameArt search not yet implemented (no public API — needs scraper).";
        if (onDone) onDone(r);
        return 0;
    }

    RequestId OpenGameArtSource::Download(const AZStd::string& /*assetId*/, DownloadCallback onDone)
    {
        DownloadResult r;
        r.success = false;
        r.errorMessage = "OpenGameArt download not yet implemented.";
        if (onDone) onDone(r);
        return 0;
    }
}
