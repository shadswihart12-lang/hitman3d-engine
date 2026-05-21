// ═══════════════════════════════════════════════════════════════
// ScriptAction.h — Generates Lua scripts from natural language
// ═══════════════════════════════════════════════════════════════

#pragma once

#include "ActionBus.h"
#include <AzCore/Component/Component.h>

namespace Hitman3D::Director
{
    class ScriptAction
        : public AZ::Component
        , protected DirectorActionBus::Handler
    {
    public:
        AZ_COMPONENT(ScriptAction, "{E5F6A7B8-C9D0-1234-EFAB-345678901234}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        void Activate() override;
        void Deactivate() override;

        bool CanHandle(IntentKind kind) const override;
        CommandResult Execute(const ParsedIntent& intent) override;
    };
}
