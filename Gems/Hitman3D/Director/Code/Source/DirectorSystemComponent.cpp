// ═══════════════════════════════════════════════════════════════
// DirectorSystemComponent.cpp
// ═══════════════════════════════════════════════════════════════

#include "DirectorSystemComponent.h"
#include "Intent/IntentParser.h"
#include "Actions/ActionBus.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Console/ILogger.h>

namespace Hitman3D::Director
{
    void DirectorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<DirectorSystemComponent, AZ::Component>()
                ->Version(1);

            if (auto* ec = sc->GetEditContext())
            {
                ec->Class<DirectorSystemComponent>(
                    "Hitman 3d Director",
                    "AI-driven game development — speak naturally, build games.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Hitman 3d")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("System"));
            }
        }

        if (auto* bc = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            bc->EBus<DirectorBus>("DirectorBus")
                ->Attribute(AZ::Script::Attributes::Category, "Hitman3D/Director")
                ->Event("SubmitCommand", &DirectorBus::Events::SubmitCommand)
                ->Event("PreviewCommand", &DirectorBus::Events::PreviewCommand)
                ->Event("UndoLast", &DirectorBus::Events::UndoLast)
                ->Event("RedoLast", &DirectorBus::Events::RedoLast)
                ->Event("GetCommandHistory", &DirectorBus::Events::GetCommandHistory);
        }
    }

    void DirectorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Hitman3DDirectorService"));
    }

    void DirectorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Hitman3DDirectorService"));
    }

    void DirectorSystemComponent::Activate()
    {
        DirectorBus::Handler::BusConnect();
        AZLOG_INFO("[Director] Hitman 3d Director activated — ready for commands.");
    }

    void DirectorSystemComponent::Deactivate()
    {
        DirectorBus::Handler::BusDisconnect();
    }

    ParsedIntent DirectorSystemComponent::PreviewCommand(const AZStd::string& naturalLanguage)
    {
        return IntentParser::Parse(naturalLanguage);
    }

    CommandId DirectorSystemComponent::SubmitCommand(const AZStd::string& naturalLanguage)
    {
        CommandId id = m_nextCommandId++;
        m_history.push_back(naturalLanguage);

        AZLOG_INFO("[Director] Command #%llu: '%s'", static_cast<unsigned long long>(id), naturalLanguage.c_str());

        ParsedIntent intent = IntentParser::Parse(naturalLanguage);

        // Notify listeners that a command was accepted
        DirectorNotificationBus::Broadcast(
            &DirectorNotifications::OnCommandAccepted, id, intent);

        // Handle special meta-intents
        if (intent.kind == IntentKind::Undo)
        {
            UndoLast();
            CommandResult r{ true, "Undo performed." };
            r.commandId = id;
            DirectorNotificationBus::Broadcast(&DirectorNotifications::OnCommandCompleted, id, r);
            return id;
        }
        if (intent.kind == IntentKind::Redo)
        {
            RedoLast();
            CommandResult r{ true, "Redo performed." };
            r.commandId = id;
            DirectorNotificationBus::Broadcast(&DirectorNotifications::OnCommandCompleted, id, r);
            return id;
        }
        if (intent.kind == IntentKind::Help)
        {
            CommandResult r;
            r.success = true;
            r.commandId = id;
            r.message = "I can help you build your game. Try commands like:\n"
                        "  - \"create a box\"\n"
                        "  - \"add a light at 0 10 0\"\n"
                        "  - \"move 'Player' to 5 0 5\"\n"
                        "  - \"create level 'MyLevel'\"\n"
                        "  - \"generate a script that makes the player jump\"\n"
                        "  - \"undo\"";
            DirectorNotificationBus::Broadcast(&DirectorNotifications::OnCommandCompleted, id, r);
            return id;
        }

        if (intent.kind == IntentKind::Unknown)
        {
            AZStd::string err = AZStd::string::format(
                "I didn't understand '%s'. Try 'help' to see what I can do.", naturalLanguage.c_str());
            DirectorNotificationBus::Broadcast(&DirectorNotifications::OnCommandFailed, id, err);
            return id;
        }

        // Dispatch to action handlers
        CommandResult result = DispatchToActions(intent);
        result.commandId = id;

        if (result.success)
        {
            DirectorNotificationBus::Broadcast(&DirectorNotifications::OnCommandCompleted, id, result);
        }
        else
        {
            DirectorNotificationBus::Broadcast(&DirectorNotifications::OnCommandFailed, id, result.message);
        }

        return id;
    }

    CommandResult DirectorSystemComponent::DispatchToActions(const ParsedIntent& intent)
    {
        CommandResult result;
        result.success = false;
        result.message = "No action handler found for this command.";

        DirectorActionBus::EnumerateHandlers(
            [&intent, &result](DirectorActionRequests* handler) -> bool
            {
                if (handler->CanHandle(intent.kind))
                {
                    result = handler->Execute(intent);
                    return false; // stop after first match
                }
                return true; // keep looking
            });

        return result;
    }

    bool DirectorSystemComponent::UndoLast()
    {
        bool undone = false;
        DirectorActionBus::EnumerateHandlers(
            [&undone](DirectorActionRequests* handler) -> bool
            {
                if (handler->Undo())
                {
                    undone = true;
                    return false;
                }
                return true;
            });

        if (undone)
        {
            AZLOG_INFO("[Director] Undo successful.");
        }
        return undone;
    }

    bool DirectorSystemComponent::RedoLast()
    {
        bool redone = false;
        DirectorActionBus::EnumerateHandlers(
            [&redone](DirectorActionRequests* handler) -> bool
            {
                if (handler->Redo())
                {
                    redone = true;
                    return false;
                }
                return true;
            });
        return redone;
    }

    AZStd::vector<AZStd::string> DirectorSystemComponent::GetCommandHistory() const
    {
        return m_history;
    }
}
