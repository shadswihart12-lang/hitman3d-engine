// ═══════════════════════════════════════════════════════════════
// TerrainAction.h — Terrain creation and modification
// ═══════════════════════════════════════════════════════════════

#pragma once

#include "ActionBus.h"
#include <AzCore/Component/Component.h>

namespace Hitman3D::Director
{
    class TerrainAction
        : public AZ::Component
        , protected DirectorActionBus::Handler
    {
    public:
        AZ_COMPONENT(TerrainAction, "{F6A7B8C9-D0E1-2345-FABC-456789012345}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        void Activate() override;
        void Deactivate() override;

        bool CanHandle(IntentKind kind) const override;
        CommandResult Execute(const ParsedIntent& intent) override;
    };
}
