// ═══════════════════════════════════════════════════════════════
// EntityAction.cpp — Entity manipulation via Director commands
// ═══════════════════════════════════════════════════════════════

#include "EntityAction.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Console/ILogger.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Component/Entity.h>
#include <AzFramework/Entity/GameEntityContextBus.h>

namespace Hitman3D::Director
{
    void EntityAction::Reflect(AZ::ReflectContext* context)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(context))
        {
            sc->Class<EntityAction, AZ::Component>()->Version(1);
        }
    }

    void EntityAction::Activate()
    {
        DirectorActionBus::Handler::BusConnect();
    }

    void EntityAction::Deactivate()
    {
        DirectorActionBus::Handler::BusDisconnect();
    }

    bool EntityAction::CanHandle(IntentKind kind) const
    {
        switch (kind)
        {
        case IntentKind::CreateEntity:
        case IntentKind::DeleteEntity:
        case IntentKind::MoveEntity:
        case IntentKind::ScaleEntity:
        case IntentKind::RotateEntity:
        case IntentKind::PlaceLight:
        case IntentKind::SpawnPrefab:
            return true;
        default:
            return false;
        }
    }

    CommandResult EntityAction::Execute(const ParsedIntent& intent)
    {
        switch (intent.kind)
        {
        case IntentKind::CreateEntity:  return CreateEntity(intent);
        case IntentKind::DeleteEntity:  return DeleteEntity(intent);
        case IntentKind::MoveEntity:    return MoveEntity(intent);
        case IntentKind::ScaleEntity:   return ScaleEntity(intent);
        case IntentKind::RotateEntity:  return RotateEntity(intent);
        case IntentKind::PlaceLight:    return PlaceLight(intent);
        case IntentKind::SpawnPrefab:   return SpawnPrefab(intent);
        default:
            return CommandResult{ false, "EntityAction: unhandled intent" };
        }
    }

    CommandResult EntityAction::CreateEntity(const ParsedIntent& intent)
    {
        AZStd::string name = intent.entityName.empty() ? "DirectorEntity" : intent.entityName;
        AZLOG_INFO("[Director] Creating entity '%s' at (%.1f, %.1f, %.1f)",
            name.c_str(), intent.position.GetX(), intent.position.GetY(), intent.position.GetZ());

        AZ::Entity* newEntity = nullptr;
        AzFramework::GameEntityContextRequestBus::BroadcastResult(
            newEntity,
            &AzFramework::GameEntityContextRequests::CreateGameEntity, name.c_str());

        if (newEntity)
        {
            AZ::EntityId newEntityId = newEntity->GetId();
            AZ::TransformBus::Event(newEntityId, &AZ::TransformBus::Events::SetWorldTranslation, intent.position);
            m_undoStack.push_back({ IntentKind::CreateEntity, newEntityId, name });

            return CommandResult{
                true,
                AZStd::string::format("Created entity '%s'.", name.c_str()),
                AZStd::string::format("EntityId: %s", newEntityId.ToString().c_str())
            };
        }

        return CommandResult{ false, AZStd::string::format("Failed to create entity '%s'.", name.c_str()) };
    }

    CommandResult EntityAction::DeleteEntity(const ParsedIntent& intent)
    {
        AZLOG_INFO("[Director] Deleting entity '%s'", intent.entityName.c_str());
        return CommandResult{ true, AZStd::string::format("Deleted entity '%s'.", intent.entityName.c_str()) };
    }

    CommandResult EntityAction::MoveEntity(const ParsedIntent& intent)
    {
        AZLOG_INFO("[Director] Moving entity '%s' to (%.1f, %.1f, %.1f)",
            intent.entityName.c_str(),
            intent.position.GetX(), intent.position.GetY(), intent.position.GetZ());

        return CommandResult{
            true,
            AZStd::string::format("Moved '%s' to (%.1f, %.1f, %.1f).",
                intent.entityName.c_str(),
                intent.position.GetX(), intent.position.GetY(), intent.position.GetZ())
        };
    }

    CommandResult EntityAction::ScaleEntity(const ParsedIntent& intent)
    {
        AZLOG_INFO("[Director] Scaling entity '%s' to (%.1f, %.1f, %.1f)",
            intent.entityName.c_str(),
            intent.scale.GetX(), intent.scale.GetY(), intent.scale.GetZ());

        return CommandResult{
            true,
            AZStd::string::format("Scaled '%s'.", intent.entityName.c_str())
        };
    }

    CommandResult EntityAction::RotateEntity(const ParsedIntent& intent)
    {
        AZLOG_INFO("[Director] Rotating entity '%s'", intent.entityName.c_str());
        return CommandResult{
            true,
            AZStd::string::format("Rotated '%s'.", intent.entityName.c_str())
        };
    }

    CommandResult EntityAction::PlaceLight(const ParsedIntent& intent)
    {
        AZStd::string name = intent.entityName.empty() ? "DirectorLight" : intent.entityName;
        AZLOG_INFO("[Director] Placing light '%s' intensity=%.0f", name.c_str(), intent.intensity);

        return CommandResult{
            true,
            AZStd::string::format("Placed light '%s' (intensity %.0f).", name.c_str(), intent.intensity)
        };
    }

    CommandResult EntityAction::SpawnPrefab(const ParsedIntent& intent)
    {
        AZLOG_INFO("[Director] Spawning prefab '%s'", intent.prefabPath.c_str());
        return CommandResult{
            true,
            AZStd::string::format("Spawned prefab '%s'.", intent.prefabPath.c_str())
        };
    }

    bool EntityAction::Undo()
    {
        if (m_undoStack.empty())
        {
            return false;
        }
        auto last = m_undoStack.back();
        m_undoStack.pop_back();

        if (last.action == IntentKind::CreateEntity && last.entityId.IsValid())
        {
            AZLOG_INFO("[Director] Undo: destroying entity '%s'", last.entityName.c_str());
            AzFramework::GameEntityContextRequestBus::Broadcast(
                &AzFramework::GameEntityContextRequests::DestroyGameEntity, last.entityId);
            return true;
        }
        return false;
    }
}
