// ═══════════════════════════════════════════════════════════════
// DirectorEditorModule.cpp — Editor-side module entry point
// ═══════════════════════════════════════════════════════════════

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "DirectorEditorSystemComponent.h"
#include "../DirectorSystemComponent.h"
#include "../Actions/SceneAction.h"
#include "../Actions/EntityAction.h"
#include "../Actions/ScriptAction.h"
#include "../Actions/TerrainAction.h"

namespace Hitman3D::Director
{
    class DirectorEditorModule : public AZ::Module
    {
    public:
        AZ_RTTI(DirectorEditorModule, "{22334455-6677-8899-AABB-CCDDEEFF0011}", AZ::Module);
        AZ_CLASS_ALLOCATOR(DirectorEditorModule, AZ::SystemAllocator);

        DirectorEditorModule()
            : AZ::Module()
        {
            m_descriptors.insert(m_descriptors.end(), {
                DirectorSystemComponent::CreateDescriptor(),
                DirectorEditorSystemComponent::CreateDescriptor(),
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
                azrtti_typeid<DirectorEditorSystemComponent>(),
                azrtti_typeid<SceneAction>(),
                azrtti_typeid<EntityAction>(),
                azrtti_typeid<ScriptAction>(),
                azrtti_typeid<TerrainAction>(),
            };
        }
    };
}

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), Hitman3D::Director::DirectorEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_Hitman3D_Director_Editor, Hitman3D::Director::DirectorEditorModule)
#endif
