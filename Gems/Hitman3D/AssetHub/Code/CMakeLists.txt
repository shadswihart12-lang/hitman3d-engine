# ═══════════════════════════════════════════════════════════════
# Hitman3D_AssetHub — Code CMakeLists.txt
# ═══════════════════════════════════════════════════════════════

# Static library — the runtime module (used by the game side)
ly_add_target(
    NAME ${gem_name}.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        assethub_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PUBLIC
            AZ::AzCore
            AZ::AzFramework
            Gem::Atom_RPI.Public
)

# The shared module loaded by the engine at runtime
ly_add_target(
    NAME ${gem_name} ${PAL_TRAIT_MONOLITHIC_GAME_SUPPORTED_DEFAULT_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        assethub_shared_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::${gem_name}.Static
)

# Editor Qt panel (AssetHubMainWindow, etc.) is not implemented yet — see AssetHub/README.md.
# Re-enable this block once Source/Editor/* and AssetHubEditorModule.cpp exist.
# if(PAL_TRAIT_BUILD_HOST_TOOLS)
#     ...
# endif()

# Game-context alias
ly_create_alias(NAME ${gem_name}.Clients NAMESPACE Gem TARGETS Gem::${gem_name})
ly_create_alias(NAME ${gem_name}.Servers NAMESPACE Gem TARGETS Gem::${gem_name})
