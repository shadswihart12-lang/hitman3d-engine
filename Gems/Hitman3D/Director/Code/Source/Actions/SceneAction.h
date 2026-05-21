// ═══════════════════════════════════════════════════════════════
// SceneAction.h — Handles level creation, saving, skybox
// ═══════════════════════════════════════════════════════════════

#pragma once

#include "ActionBus.h"
#include <AzCore/Component/Component.h>

namespace Hitman3D::Director
{
    class SceneAction
        : public AZ::Component
        , protected DirectorActionBus::Handler
    {
    public:
        AZ_COMPONENT(SceneAction, "{C3D4E5F6-A7B8-9012-CDEF-123456789012}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        void Activate() override;
        void Deactivate() override;

        bool CanHandle(IntentKind kind) const override;
        CommandResult Execute(const ParsedIntent& intent) override;
    };
}
