// ═══════════════════════════════════════════════════════════════
// AssetHubModule.cpp
// Gem module — O3DE's entry point that registers the system component.
// ═══════════════════════════════════════════════════════════════

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "AssetHubSystemComponent.h"

namespace Hitman3D::AssetHub
{
    class AssetHubModule : public AZ::Module
    {
    public:
        AZ_RTTI(AssetHubModule, "{77889900-aaaa-bbbb-cccc-ddddeeeeffff}", AZ::Module);
        AZ_CLASS_ALLOCATOR(AssetHubModule, AZ::SystemAllocator);

        AssetHubModule()
            : AZ::Module()
        {
            m_descriptors.insert(m_descriptors.end(), {
                AssetHubSystemComponent::CreateDescriptor(),
            });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<AssetHubSystemComponent>(),
            };
        }
    };
}

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), Hitman3D::AssetHub::AssetHubModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_Hitman3D_AssetHub, Hitman3D::AssetHub::AssetHubModule)
#endif
