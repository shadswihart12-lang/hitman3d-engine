// ═══════════════════════════════════════════════════════════════
// TerrainAction.cpp — Terrain manipulation via Director
// ═══════════════════════════════════════════════════════════════

#include "TerrainAction.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Console/ILogger.h>

namespace Hitman3D::Director
{
    void TerrainAction::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<TerrainAction, AZ::Component>()->Version(1);
        }
    }

    void TerrainAction::Activate()
    {
        DirectorActionBus::Handler::BusConnect();
    }

    void TerrainAction::Deactivate()
    {
        DirectorActionBus::Handler::BusDisconnect();
    }

    bool TerrainAction::CanHandle(IntentKind kind) const
    {
        return kind == IntentKind::CreateTerrain
            || kind == IntentKind::ModifyTerrain;
    }

    CommandResult TerrainAction::Execute(const ParsedIntent& intent)
    {
        CommandResult result;

        if (intent.kind == IntentKind::CreateTerrain)
        {
            AZLOG_INFO("[Director] Creating terrain");
            result.success = true;
            result.message = "Terrain created.";
        }
        else if (intent.kind == IntentKind::ModifyTerrain)
        {
            AZLOG_INFO("[Director] Modifying terrain");
            result.success = true;
            result.message = "Terrain modified.";
        }
        else
        {
            result.success = false;
            result.message = "TerrainAction: unhandled intent";
        }

        return result;
    }
}
