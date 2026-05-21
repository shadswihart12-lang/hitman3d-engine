// ═══════════════════════════════════════════════════════════════
// ActionBus.h — Internal bus for dispatching parsed intents
// to concrete action handlers.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/EBus/EBus.h>
#include <Hitman3D/Director/DirectorTypes.h>

namespace Hitman3D::Director
{
    class DirectorActionRequests : public AZ::EBusTraits
    {
    public:
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual ~DirectorActionRequests() = default;

        //! Returns true if this handler can execute the given intent kind.
        virtual bool CanHandle(IntentKind kind) const = 0;

        //! Execute the action. Returns a result describing success/failure.
        virtual CommandResult Execute(const ParsedIntent& intent) = 0;

        //! Undo the last action this handler performed.
        virtual bool Undo() { return false; }

        //! Redo the last undone action.
        virtual bool Redo() { return false; }
    };
    using DirectorActionBus = AZ::EBus<DirectorActionRequests>;
}
