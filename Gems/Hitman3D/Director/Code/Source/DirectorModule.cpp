// ═══════════════════════════════════════════════════════════════
// DirectorModule.cpp — Gem entry point
// ═══════════════════════════════════════════════════════════════

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "DirectorSystemComponent.h"
#include "Actions/SceneAction.h"
#include "Actions/EntityAction.h"
#include "Actions/ScriptAction.h"
#include "Actions/TerrainAction.h"

namespace Hitman3D::Director
{
    class DirectorModule : public AZ::Module
    {
    public:
        AZ_RTTI(DirectorModule, "{11223344-5566-7788-99AA-BBCCDDEEFF00}", AZ::Module);
        AZ_CLASS_ALLOCATOR(DirectorModule, AZ::SystemAllocator);

        DirectorModule()
            : AZ::Module()
        {
            m_descriptors.insert(m_descriptors.end(), {
                DirectorSystemComponent::CreateDescriptor(),
                SceneAction::CreateDescriptor(),
                EntityAction::CreateDescriptor(),
                ScriptAction::CreateDescriptor(),
                TerrainAction::CreateDescriptor(),
            });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<DirectorSystemComponent>(),
                azrtti_typeid<SceneAction>(),
                azrtti_typeid<EntityAction>(),
                azrtti_typeid<ScriptAction>(),
                azrtti_typeid<TerrainAction>(),
            };
        }
    };
}

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), Hitman3D::Director::DirectorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_Hitman3D_Director, Hitman3D::Director::DirectorModule)
#endif
