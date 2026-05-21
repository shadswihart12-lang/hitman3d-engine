// ═══════════════════════════════════════════════════════════════
// DirectorEditorSystemComponent.cpp
// ═══════════════════════════════════════════════════════════════

#include "DirectorEditorSystemComponent.h"
#include "DirectorWidget.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzToolsFramework/API/ViewPaneOptions.h>

namespace Hitman3D::Director
{
    static constexpr const char* DirectorPanelName = "Director";

    void DirectorEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<DirectorEditorSystemComponent, AZ::Component>()
                ->Version(1);

            if (auto* ec = sc->GetEditContext())
            {
                ec->Class<DirectorEditorSystemComponent>(
                    "Hitman 3d Director (Editor)",
                    "Registers the Director AI panel in the editor.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Hitman 3d")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("System"));
            }
        }
    }

    void DirectorEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Hitman3DDirectorEditorService"));
    }

    void DirectorEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Hitman3DDirectorEditorService"));
    }

    void DirectorEditorSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("Hitman3DDirectorService"));
    }

    void DirectorEditorSystemComponent::Activate()
    {
        AzToolsFramework::ViewPaneOptions options;
        options.paneRect = QRect(50, 50, 420, 600);
        options.isDeletable = false;
        options.canHaveMultipleInstances = false;

        AzToolsFramework::RegisterViewPane<DirectorWidget>(DirectorPanelName, "Hitman 3d", options);
    }

    void DirectorEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::UnregisterViewPane(DirectorPanelName);
    }
}
