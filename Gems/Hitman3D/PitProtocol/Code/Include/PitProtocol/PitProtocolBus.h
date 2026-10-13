/*
 * PitProtocolBus.h — public EBus API + domain types for PIT PROTOCOL.
 * The Pit, Phase A: no-rounds bouts, limb state maps, desperation verbs,
 * walk-out decisions. Doctrine: docs/PIT_PROTOCOL.md.
 * (House pattern per AssetHubBus.h: Requests interface + traits + EBus alias.)
 */
#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/string/string.h>

#include <cstdint>

namespace PitProtocol
{
    //! Bout law (doctrine): ONE continuous arc — no rounds. Damage
    //! compounds; momentum never resets; the pit only escalates.
    enum class BoutPhase : uint8_t
    {
        Idle,       //!< no bout registered
        Active,     //!< continuous combat arc in progress
        Walkout,    //!< a fighter walked out (honor preserved, streak ends)
        Ended       //!< terminal state (squad-wipe or submission)
    };

    //! Limb state map — the anatomical strategy layer.
    enum class LimbState : uint8_t
    {
        Healthy,
        Weakened,
        Broken      //!< target what's broken — desperation verbs feed here
    };

    //! Which limb a strike or desperation verb targets.
    enum class Limb : uint8_t
    {
        LeftArm,
        RightArm,
        LeftLeg,
        RightLeg,
        Head
    };

    //! Desperation verbs — the ugly moves with costs (doctrine pillar 3).
    enum class DesperationVerb : uint8_t
    {
        None,
        Bite,       //!< last-resort reversal; targets what's broken; costs position
        Drown       //!< pin in the kill-zone
    };

    //! Snapshot of one fighter inside the bout (read-only view for
    //! UI/AI/spectator systems).
    struct FighterSnapshot
    {
        AZ_TYPE_INFO(FighterSnapshot, "{7C1D0A4E-1F2B-4A61-9B2E-F16A7E555AAB}");

        AZStd::string m_name;
        float m_stamina{ 1.0f };            // 0..1 — drains continuously (no rounds)
        float m_health{ 1.0f };             // 0..1 — compounds, never resets
        LimbState m_leftArm{ LimbState::Healthy };
        LimbState m_rightArm{ LimbState::Healthy };
        LimbState m_leftLeg{ LimbState::Healthy };
        LimbState m_rightLeg{ LimbState::Healthy };
        LimbState m_head{ LimbState::Healthy };
        bool m_downed{ false };
        bool m_drowning{ false };           // kill-zone state (Phase A: stamina drain + posture)
    };

    //! High-level bout operations (game-side systems, AI houses, Director).
    class PitProtocolRequests
    {
    public:
        AZ_RTTI(PitProtocolRequests, "{3E5F6A7B-8C9D-4E0F-A1B2-C3D4E5F60718}");
        virtual ~PitProtocolRequests() = default;

        //! Bout lifecycle — no rounds: StartBout once, EndBout once.
        virtual void StartBout(const AZStd::string& fighterA, const AZStd::string& fighterB) = 0;
        virtual BoutPhase GetBoutPhase() const = 0;

        //! Register/read fighters inside the active bout.
        virtual void RegisterFighter(const FighterSnapshot& fighter) = 0;
        virtual bool GetFighterSnapshot(const AZStd::string& name, FighterSnapshot& outSnapshot) const = 0;

        //! Combat verbs. Compound damage: every hit persists (no-rounds law).
        virtual void ApplyStrike(const AZStd::string& attacker, const AZStd::string& defender, Limb limb, float force) = 0;

        //! Desperation verbs — ugly moves with costs.
        virtual void ApplyDesperation(const AZStd::string& attacker, const AZStd::string& defender, DesperationVerb verb) = 0;

        //! The champion's choice (doctrine pillar 5): fight on or walk out.
        virtual void WalkOut(const AZStd::string& fighter) = 0;
        virtual void EndBout(const AZStd::string& reason) = 0;
    };

    //! EBus traits — single bus, single handler (the system component).
    class PitProtocolBusTraits : public AZ::EBusTraits
    {
    public:
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
    };

    using PitProtocolBus = AZ::EBus<PitProtocolRequests, PitProtocolBusTraits>;
} // namespace PitProtocol
