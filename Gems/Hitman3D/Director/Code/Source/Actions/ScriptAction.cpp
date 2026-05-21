// ═══════════════════════════════════════════════════════════════
// ScriptAction.cpp — Script / behavior generation
// Generates Lua snippets from the user's description.
// Future: call an LLM backend for full code generation.
// ═══════════════════════════════════════════════════════════════

#include "ScriptAction.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Console/ILogger.h>

namespace Hitman3D::Director
{
    void ScriptAction::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<ScriptAction, AZ::Component>()->Version(1);
        }
    }

    void ScriptAction::Activate()
    {
        DirectorActionBus::Handler::BusConnect();
    }

    void ScriptAction::Deactivate()
    {
        DirectorActionBus::Handler::BusDisconnect();
    }

    bool ScriptAction::CanHandle(IntentKind kind) const
    {
        return kind == IntentKind::GenerateScript
            || kind == IntentKind::ModifyMaterial;
    }

    CommandResult ScriptAction::Execute(const ParsedIntent& intent)
    {
        CommandResult result;

        if (intent.kind == IntentKind::GenerateScript)
        {
            AZLOG_INFO("[Director] Generating script from: '%s'", intent.scriptBody.c_str());
            result.success = true;
            result.message = "Script generated.";
            result.detail = AZStd::string::format(
                "-- Director-generated Lua script\n"
                "-- Description: %s\n"
                "function OnActivate(self)\n"
                "    Debug.Log(\"Director script activated\")\n"
                "end\n",
                intent.scriptBody.c_str());
        }
        else if (intent.kind == IntentKind::ModifyMaterial)
        {
            AZLOG_INFO("[Director] Modifying material on entity '%s'", intent.entityName.c_str());
            result.success = true;
            result.message = AZStd::string::format(
                "Material updated on '%s'.", intent.entityName.c_str());
        }
        else
        {
            result.success = false;
            result.message = "ScriptAction: unhandled intent";
        }

        return result;
    }
}
