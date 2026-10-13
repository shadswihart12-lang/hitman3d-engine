/*
 * PitProtocolModule.cpp — module wiring for the PitProtocol gem.
 */
#include <PitProtocol/PitProtocolBus.h>
#include "PitProtocolSystemComponent.h"

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <AzCore/RTTI/RTTI.h>

namespace PitProtocol
{
    class PitProtocolModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(PitProtocolModule, "{B8D42E1F-AE3C-4B72-9C3F-6FBC8D2EAE02}", AZ::Module);
        AZ_CLASS_ALLOCATOR(PitProtocolModule, AZ::SystemAllocator);

        PitProtocolModule()
        {
            m_descriptors.insert(m_descriptors.end(), {
                PitProtocolSystemComponent::CreateDescriptor(),
            });
        }
    };
} // namespace PitProtocol

AZ_DECLARE_MODULE_CLASS(Gem_Hitman3D_PitProtocol, PitProtocol::PitProtocolModule)
