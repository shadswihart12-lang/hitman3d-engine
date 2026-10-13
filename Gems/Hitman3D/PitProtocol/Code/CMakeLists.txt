# ═══════════════════════════════════════════════════════════════
# Hitman3D_PitProtocol — Code CMakeLists.txt
# ═══════════════════════════════════════════════════════════════
# LESSON (campaign 1d): ly_add_target requires exactly ONE linking
# option; PAL variables are EMPTY on linux-default → GEM_MODULE only.

# Static library — the runtime logic (game-side, AI houses, sim core)
ly_add_target(
    NAME ${gem_name}.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        pitprotocol_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PUBLIC
            AZ::AzCore
            AZ::AzFramework
)

# The shared module loaded by the engine at runtime
ly_add_target(
    NAME ${gem_name} GEM_MODULE
    NAMESPACE Gem
    FILES_CMAKE
        pitprotocol_shared_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::${gem_name}.Static
            AZ::AzCore
)

# Game-context aliases (headless sim — Clients and Servers both)
ly_create_alias(NAME ${gem_name}.Clients NAMESPACE Gem TARGETS Gem::${gem_name})
ly_create_alias(NAME ${gem_name}.Servers NAMESPACE Gem TARGETS Gem::${gem_name})
