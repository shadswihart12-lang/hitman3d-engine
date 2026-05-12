// ═══════════════════════════════════════════════════════════════
// AssetMetadata.h
// Common asset descriptor returned by all IAssetSource implementations.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/Memory/Memory.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/vector.h>

namespace Hitman3D::AssetHub
{
    //! License classification — controls visibility filtering by default.
    enum class LicenseClass : AZ::u8
    {
        Unknown        = 0,
        CC0            = 1,  // public domain, no attribution
        CC_BY          = 2,  // attribution required
        CC_BY_SA       = 3,  // attribution + share-alike
        CC_BY_NC       = 4,  // non-commercial only (HIDDEN by default)
        Proprietary    = 5,  // paid / Megascans / Fab
        Custom         = 6,  // see license_text field
    };

    //! Asset category — drives importer behavior.
    enum class AssetCategory : AZ::u8
    {
        Unknown        = 0,
        StaticMesh     = 1,
        SkeletalMesh   = 2,
        Material       = 3,
        Texture        = 4,
        HDRI           = 5,
        Audio          = 6,
        Animation      = 7,
    };

    //! Descriptor returned by sources. Owned by AssetHubSystemComponent's cache.
    struct AssetMetadata
    {
        AZ_TYPE_INFO(AssetMetadata, "{c4f23c11-7b2e-4a91-95c9-21d8a1d2e445}");
        AZ_CLASS_ALLOCATOR(AssetMetadata, AZ::SystemAllocator);

        AZStd::string sourceId;          // "sketchfab" / "megascans" / "openGameArt" / "fab"
        AZStd::string assetId;            // source-local unique ID
        AZStd::string displayName;        // human-readable name
        AZStd::string author;             // original creator
        AZStd::string description;
        AZStd::string thumbnailUrl;       // direct image URL for preview
        AZStd::string originalUrl;        // link back to source page

        AssetCategory category = AssetCategory::Unknown;
        LicenseClass  license  = LicenseClass::Unknown;
        AZStd::string licenseText;        // free-text full license string

        AZ::u64 polyCount  = 0;           // 0 if unknown
        AZ::u64 fileSizeBytes = 0;        // 0 if unknown
        AZStd::vector<AZStd::string> tags;

        //! True if license permits commercial use without restrictive terms.
        bool IsCommercialSafe() const
        {
            return license == LicenseClass::CC0
                || license == LicenseClass::CC_BY
                || license == LicenseClass::Proprietary;  // user owns it
        }
    };

    //! Search query passed to IAssetSource::Search.
    struct SearchQuery
    {
        AZStd::string text;
        AssetCategory categoryFilter = AssetCategory::Unknown;  // Unknown = any
        bool commercialSafeOnly = true;
        AZ::u32 maxResults = 50;
        AZ::u32 pageOffset = 0;
    };
}
