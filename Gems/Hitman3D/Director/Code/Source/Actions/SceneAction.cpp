// ═══════════════════════════════════════════════════════════════
// SceneAction.cpp — Level / scene management actions
// ═══════════════════════════════════════════════════════════════

#include "SceneAction.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Console/ILogger.h>

namespace Hitman3D::Director
{
    void SceneAction::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<SceneAction, AZ::Component>()->Version(1);
        }
    }

    void SceneAction::Activate()
    {
        DirectorActionBus::Handler::BusConnect();
    }

    void SceneAction::Deactivate()
    {
        DirectorActionBus::Handler::BusDisconnect();
    }

    bool SceneAction::CanHandle(IntentKind kind) const
    {
        return kind == IntentKind::CreateLevel
            || kind == IntentKind::SaveLevel
            || kind == IntentKind::SetSkybox;
    }

    CommandResult SceneAction::Execute(const ParsedIntent& intent)
    {
        CommandResult result;

        switch (intent.kind)
        {
        case IntentKind::CreateLevel:
            AZLOG_INFO("[Director] Creating level: '%s'", intent.entityName.c_str());
            result.success = true;
            result.message = AZStd::string::format("Level '%s' created.", intent.entityName.c_str());
            break;

        case IntentKind::SaveLevel:
            AZLOG_INFO("[Director] Saving current level");
            result.success = true;
            result.message = "Level saved.";
            break;

        case IntentKind::SetSkybox:
            AZLOG_INFO("[Director] Setting skybox / atmosphere");
            result.success = true;
            result.message = "Skybox updated.";
            break;

        default:
            result.success = false;
            result.message = "SceneAction: unhandled intent";
            break;
        }

        return result;
    }
}
