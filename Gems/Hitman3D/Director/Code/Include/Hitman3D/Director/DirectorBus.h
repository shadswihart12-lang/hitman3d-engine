// ═══════════════════════════════════════════════════════════════
// DirectorBus.h — EBus interface for the Director AI system
// Any code (editor panels, Script Canvas, Python, other gems)
// can send natural-language commands through this bus.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/functional.h>

#include <Hitman3D/Director/DirectorTypes.h>

namespace Hitman3D::Director
{
    // ── Request bus (singleton) ──────────────────────────────
    class DirectorRequests : public AZ::EBusTraits
    {
    public:
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual ~DirectorRequests() = default;

        //! Submit a natural-language command. Returns immediately;
        //! result is delivered via the callback or the notification bus.
        virtual CommandId SubmitCommand(const AZStd::string& naturalLanguage) = 0;

        //! Parse without executing — useful for preview / confirmation UI.
        virtual ParsedIntent PreviewCommand(const AZStd::string& naturalLanguage) = 0;

        //! Undo the last Director-issued action.
        virtual bool UndoLast() = 0;

        //! Redo the last undone Director action.
        virtual bool RedoLast() = 0;

        //! Get history of commands for the current session.
        virtual AZStd::vector<AZStd::string> GetCommandHistory() const = 0;
    };
    using DirectorBus = AZ::EBus<DirectorRequests>;

    // ── Notification bus ─────────────────────────────────────
    class DirectorNotifications : public AZ::EBusTraits
    {
    public:
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Multiple;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual ~DirectorNotifications() = default;

        virtual void OnCommandAccepted(CommandId id, const ParsedIntent& intent) {}
        virtual void OnCommandCompleted(CommandId id, const CommandResult& result) {}
        virtual void OnCommandFailed(CommandId id, const AZStd::string& error) {}
    };
    using DirectorNotificationBus = AZ::EBus<DirectorNotifications>;
}
