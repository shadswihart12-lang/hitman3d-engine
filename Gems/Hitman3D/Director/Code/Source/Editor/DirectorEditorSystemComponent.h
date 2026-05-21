// ═══════════════════════════════════════════════════════════════
// DirectorEditorSystemComponent.h
// Registers the Director panel in the editor's View menu.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/Component/Component.h>
#include <AzToolsFramework/API/ViewPaneOptions.h>

namespace Hitman3D::Director
{
    class DirectorEditorSystemComponent
        : public AZ::Component
    {
    public:
        AZ_COMPONENT(DirectorEditorSystemComponent, "{B1C2D3E4-F5A6-7890-BCDE-F01234567890}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

    protected:
        void Init() override {}
        void Activate() override;
        void Deactivate() override;
    };
}
