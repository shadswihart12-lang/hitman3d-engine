# ═══════════════════════════════════════════════════════════════
# Hitman3D_Director — Code CMakeLists.txt
# ═══════════════════════════════════════════════════════════════

ly_add_target(
    NAME ${gem_name}.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        director_files.cmake
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

ly_add_target(
    NAME ${gem_name} GEM_MODULE
    NAMESPACE Gem
    FILES_CMAKE
        director_shared_files.cmake
    INCLUDE_DIRECTORIES
        PRIVATE
            Source
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::${gem_name}.Static
)

if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME ${gem_name}.Editor.Static STATIC
        NAMESPACE Gem
        AUTOMOC
        FILES_CMAKE
            director_editor_files.cmake
        INCLUDE_DIRECTORIES
            PUBLIC
                Include
            PRIVATE
                Source
        BUILD_DEPENDENCIES
            PUBLIC
                AZ::AzToolsFramework
                Gem::${gem_name}.Static
                3rdParty::Qt::Core
                3rdParty::Qt::Widgets
    )

    ly_add_target(
        NAME ${gem_name}.Editor GEM_MODULE
        NAMESPACE Gem
        FILES_CMAKE
            director_editor_shared_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
                Include
        BUILD_DEPENDENCIES
            PRIVATE
                Gem::${gem_name}.Editor.Static
    )

    ly_create_alias(NAME ${gem_name}.Tools NAMESPACE Gem TARGETS Gem::${gem_name}.Editor)
endif()

ly_create_alias(NAME ${gem_name}.Clients NAMESPACE Gem TARGETS Gem::${gem_name})
ly_create_alias(NAME ${gem_name}.Servers NAMESPACE Gem TARGETS Gem::${gem_name})
