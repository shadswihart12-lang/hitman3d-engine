# ═══════════════════════════════════════════════════════════════
# Hitman3D_AssetHub — root CMakeLists.txt
# ═══════════════════════════════════════════════════════════════

# Currently we don't have any assets in the AssetHub gem.
# o3de_pal_dir does the platform-relative resolution.

o3de_gem_setup("Hitman3D_AssetHub")

# Code module(s)
add_subdirectory(Code)
