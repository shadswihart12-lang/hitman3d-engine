// ═══════════════════════════════════════════════════════════════
// EntityAction.h — Create, delete, move, scale, rotate entities
// ═══════════════════════════════════════════════════════════════

#pragma once

#include "ActionBus.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/std/containers/vector.h>

namespace Hitman3D::Director
{
    class EntityAction
        : public AZ::Component
        , protected DirectorActionBus::Handler
    {
    public:
        AZ_COMPONENT(EntityAction, "{D4E5F6A7-B8C9-0123-DEFA-234567890123}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        void Activate() override;
        void Deactivate() override;

        bool CanHandle(IntentKind kind) const override;
        CommandResult Execute(const ParsedIntent& intent) override;
        bool Undo() override;

    private:
        CommandResult CreateEntity(const ParsedIntent& intent);
        CommandResult DeleteEntity(const ParsedIntent& intent);
        CommandResult MoveEntity(const ParsedIntent& intent);
        CommandResult ScaleEntity(const ParsedIntent& intent);
        CommandResult RotateEntity(const ParsedIntent& intent);
        CommandResult PlaceLight(const ParsedIntent& intent);
        CommandResult SpawnPrefab(const ParsedIntent& intent);

        struct UndoRecord
        {
            IntentKind action;
            AZ::EntityId entityId;
            AZStd::string entityName;
        };
        AZStd::vector<UndoRecord> m_undoStack;
    };
}
