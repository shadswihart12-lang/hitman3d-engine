/*
 * PitProtocolSystemComponent.cpp — the bout state machine heart of
 * PIT PROTOCOL (The Pit, Phase A). Every function below encodes a
 * doctrine law from docs/PIT_PROTOCOL.md.
 */
#include "PitProtocolSystemComponent.h"

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>

namespace PitProtocol
{
    void PitProtocolSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<PitProtocolSystemComponent, AZ::Component>()
                ->Version(1);
        }
    }

    void PitProtocolSystemComponent::Activate()
    {
        PitProtocolBus::Handler::BusConnect();
    }

    void PitProtocolSystemComponent::Deactivate()
    {
        PitProtocolBus::Handler::BusDisconnect();
    }

    FighterSnapshot* PitProtocolSystemComponent::FindFighter(const AZStd::string& name)
    {
        for (auto& fighter : m_fighters)
        {
            if (fighter.m_name == name)
            {
                return &fighter;
            }
        }
        return nullptr;
    }

    const FighterSnapshot* PitProtocolSystemComponent::FindFighter(const AZStd::string& name) const
    {
        for (const auto& fighter : m_fighters)
        {
            if (fighter.m_name == name)
            {
                return &fighter;
            }
        }
        return nullptr;
    }

    void PitProtocolSystemComponent::StartBout(const AZStd::string& fighterA, const AZStd::string& fighterB)
    {
        if (m_phase == BoutPhase::Active)
        {
            return; // one continuous arc: no nested bouts, no restarts mid-arc
        }

        m_fighters.clear();
        m_endReason.clear();

        FighterSnapshot a;
        a.m_name = fighterA;
        FighterSnapshot b;
        b.m_name = fighterB;
        m_fighters.push_back(AZStd::move(a));
        m_fighters.push_back(AZStd::move(b));

        m_phase = BoutPhase::Active;
    }

    BoutPhase PitProtocolSystemComponent::GetBoutPhase() const
    {
        return m_phase;
    }

    void PitProtocolSystemComponent::RegisterFighter(const FighterSnapshot& fighter)
    {
        if (m_phase != BoutPhase::Active || m_fighters.full())
        {
            return;
        }
        m_fighters.push_back(fighter);
    }

    bool PitProtocolSystemComponent::GetFighterSnapshot(const AZStd::string& name, FighterSnapshot& outSnapshot) const
    {
        if (const FighterSnapshot* found = FindFighter(name))
        {
            outSnapshot = *found;
            return true;
        }
        return false;
    }

    void PitProtocolSystemComponent::ApplyLimbDamage(FighterSnapshot& fighter, Limb limb, float force)
    {
        LimbState* state = nullptr;
        switch (limb)
        {
        case Limb::LeftArm:
            state = &fighter.m_leftArm;
            break;
        case Limb::RightArm:
            state = &fighter.m_rightArm;
            break;
        case Limb::LeftLeg:
            state = &fighter.m_leftLeg;
            break;
        case Limb::RightLeg:
            state = &fighter.m_rightLeg;
            break;
        case Limb::Head:
            state = &fighter.m_head;
            break;
        }

        if (!state)
        {
            return;
        }

        // Doctrine: damage compounds (no-rounds law). Healthy -> Weakened
        // -> Broken; Broken limbs make desperation verbs lethal.
        switch (*state)
        {
        case LimbState::Healthy:
            *state = LimbState::Weakened;
            break;
        case LimbState::Weakened:
            *state = LimbState::Broken;
            break;
        case LimbState::Broken:
        default:
            break;
        }

        fighter.m_health = AZ::GetClamp(fighter.m_health - force * 0.1f, 0.0f, 1.0f);
    }

    void PitProtocolSystemComponent::ApplyStrike(const AZStd::string& attacker, const AZStd::string& defender, Limb limb, float force)
    {
        if (m_phase != BoutPhase::Active)
        {
            return;
        }

        FighterSnapshot* attackerSnapshot = FindFighter(attacker);
        FighterSnapshot* defenderSnapshot = FindFighter(defender);
        if (!attackerSnapshot || !defenderSnapshot)
        {
            return;
        }

        // Attacking costs stamina (continuous drain — no-rounds law).
        attackerSnapshot->m_stamina = AZ::GetClamp(attackerSnapshot->m_stamina - force * 0.05f, 0.0f, 1.0f);

        ApplyLimbDamage(*defenderSnapshot, limb, force);

        CheckTerminalState();
    }

    void PitProtocolSystemComponent::ApplyDesperation(const AZStd::string& attacker, const AZStd::string& defender, DesperationVerb verb)
    {
        if (m_phase != BoutPhase::Active)
        {
            return;
        }

        FighterSnapshot* attackerSnapshot = FindFighter(attacker);
        FighterSnapshot* defenderSnapshot = FindFighter(defender);
        if (!attackerSnapshot || !defenderSnapshot)
        {
            return;
        }

        switch (verb)
        {
        case DesperationVerb::Bite:
            // Doctrine: desperation targets what's broken — a bite on a
            // Broken limb is devastating; on Healthy flesh it's survivable.
            {
                const bool anyBroken = defenderSnapshot->m_leftArm == LimbState::Broken ||
                    defenderSnapshot->m_rightArm == LimbState::Broken ||
                    defenderSnapshot->m_leftLeg == LimbState::Broken ||
                    defenderSnapshot->m_rightLeg == LimbState::Broken ||
                    defenderSnapshot->m_head == LimbState::Broken;
                const float damage = anyBroken ? 0.35f : 0.12f;
                defenderSnapshot->m_health = AZ::GetClamp(defenderSnapshot->m_health - damage, 0.0f, 1.0f);
                // Cost: position loss — the attacker bleeds stamina for the ugly move.
                attackerSnapshot->m_stamina = AZ::GetClamp(attackerSnapshot->m_stamina - 0.2f, 0.0f, 1.0f);
            }
            break;

        case DesperationVerb::Drown:
            // Kill-zone pin: stamina drain + posture collapse.
            defenderSnapshot->m_drowning = true;
            defenderSnapshot->m_stamina = AZ::GetClamp(defenderSnapshot->m_stamina - 0.25f, 0.0f, 1.0f);
            break;

        case DesperationVerb::None:
        default:
            break;
        }

        CheckTerminalState();
    }

    void PitProtocolSystemComponent::WalkOut(const AZStd::string& fighter)
    {
        // Doctrine pillar 5: the walk-out is a first-class outcome —
        // streak ends, honor preserved. Not a loss.
        if (m_phase != BoutPhase::Active)
        {
            return;
        }
        if (FindFighter(fighter))
        {
            m_endReason = AZStd::string::format("%s walked out.", fighter.c_str());
            m_phase = BoutPhase::Walkout;
        }
    }

    void PitProtocolSystemComponent::CheckTerminalState()
    {
        if (m_phase != BoutPhase::Active)
        {
            return;
        }

        // Doctrine: the bout ends when the fighter STOPS — health zero,
        // stamina zero (couldn't continue), or downed.
        for (const auto& fighter : m_fighters)
        {
            if (fighter.m_health <= 0.0f || (fighter.m_stamina <= 0.0f && fighter.m_drowning))
            {
                m_endReason = AZStd::string::format("%s could not continue.", fighter.m_name.c_str());
                m_phase = BoutPhase::Ended;
                return;
            }
        }
    }

    void PitProtocolSystemComponent::EndBout(const AZStd::string& reason)
    {
        m_endReason = reason;
        m_phase = BoutPhase::Ended;
    }
} // namespace PitProtocol
