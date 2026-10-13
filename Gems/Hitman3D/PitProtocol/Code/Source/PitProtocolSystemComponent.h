/*
 * PitProtocolSystemComponent.h — the bout state machine heart of
 * PIT PROTOCOL (The Pit, Phase A). Doctrine: docs/PIT_PROTOCOL.md.
 */
#pragma once

#include <PitProtocol/PitProtocolBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/std/containers/fixed_vector.h>
#include <AzCore/std/string/string.h>

namespace PitProtocol
{
    //! No-rounds bout engine (Phase A: systems truth, art later).
    //! Doctrine laws encoded here:
    //!  - ONE continuous arc: damage compounds, nothing resets per "round"
    //!  - limb state map: strikes target anatomy, desperation verbs
    //!    target what's broken
    //!  - stamina drains continuously; drowning/kill-zone drains faster
    //!  - the walk-out is a first-class outcome (honor preserved)
    class PitProtocolSystemComponent
        : public AZ::Component
        , public PitProtocolBus::Handler
    {
    public:
        AZ_COMPONENT(PitProtocolSystemComponent, "{A7C31D0E-9F2B-4A61-8B2E-5EAB7C1D9F01}", AZ::Component);

        static void Reflect(AZ::ReflectContext* context);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;

        // PitProtocolBus
        void StartBout(const AZStd::string& fighterA, const AZStd::string& fighterB) override;
        BoutPhase GetBoutPhase() const override;
        void RegisterFighter(const FighterSnapshot& fighter) override;
        bool GetFighterSnapshot(const AZStd::string& name, FighterSnapshot& outSnapshot) const override;
        void ApplyStrike(const AZStd::string& attacker, const AZStd::string& defender, Limb limb, float force) override;
        void ApplyDesperation(const AZStd::string& attacker, const AZStd::string& defender, DesperationVerb verb) override;
        void WalkOut(const AZStd::string& fighter) override;
        void EndBout(const AZStd::string& reason) override;

    private:
        static constexpr uint8_t MaxFighters = 2; // Phase A: 1v1 bouts (team wars = Phase B)

        BoutPhase m_phase{ BoutPhase::Idle };
        AZStd::string m_endReason;

        AZStd::fixed_vector<FighterSnapshot, MaxFighters> m_fighters;

        FighterSnapshot* FindFighter(const AZStd::string& name);
        const FighterSnapshot* FindFighter(const AZStd::string& name) const;
        void ApplyLimbDamage(FighterSnapshot& fighter, Limb limb, float force);
        void CheckTerminalState();
    };
} // namespace PitProtocol
