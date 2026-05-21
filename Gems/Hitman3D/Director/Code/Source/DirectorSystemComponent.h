// ═══════════════════════════════════════════════════════════════
// DirectorSystemComponent.h
// The runtime brain — receives NL commands via DirectorBus,
// parses intents, dispatches to action handlers.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/std/containers/vector.h>

#include <Hitman3D/Director/DirectorBus.h>

namespace Hitman3D::Director
{
    class DirectorSystemComponent
        : public AZ::Component
        , protected DirectorBus::Handler
    {
    public:
        AZ_COMPONENT(DirectorSystemComponent, "{A0B1C2D3-E4F5-6789-ABCD-EF0123456789}");

        static void Reflect(AZ::ReflectContext* context);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        DirectorSystemComponent() = default;
        ~DirectorSystemComponent() override = default;

    protected:
        // AZ::Component
        void Init() override {}
        void Activate() override;
        void Deactivate() override;

        // DirectorBus::Handler
        CommandId SubmitCommand(const AZStd::string& naturalLanguage) override;
        ParsedIntent PreviewCommand(const AZStd::string& naturalLanguage) override;
        bool UndoLast() override;
        bool RedoLast() override;
        AZStd::vector<AZStd::string> GetCommandHistory() const override;

    private:
        CommandResult DispatchToActions(const ParsedIntent& intent);

        CommandId m_nextCommandId = 1;
        AZStd::vector<AZStd::string> m_history;
    };
}
