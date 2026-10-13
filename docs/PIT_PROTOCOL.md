# PIT PROTOCOL — The Pit's Phase A on Hitman3D
## JSS Studio · The in-house engine's first game. Design proof + sample project in one.

> "The banner remembers." — The Pit doctrine lives at
> `/mnt/c/Users/shads/OneDrive/Documents/Stride Projects/thepit/DESIGN.md`
> (source of truth). This document = Phase A engineering plan inside the
> engine, per the founder's ruling (2026-10-13): The Pit targets
> Hitman3D. Game and engine grow together.

## WHAT PIT PROTOCOL VALIDATES (the novel systems, abstract of 3D art)
1. NO-ROUNDS combat arc — one continuous fight, damage compounds,
   momentum never resets.
2. LIMB STATE MAP — arms/legs/head as state booleans + stamina pool;
   anatomical targeting (weaken guard arm, kill lead leg).
3. DESPERATION VERBS — bite as last-resort reversal with costs.
4. ENVIRONMENT KILL-ZONES — drown states in a liquid zone.
5. THE WALK-OUT — champion's decision mechanic (fight on vs retire with
   honor). AI houses must model this choice too.
6. HOUSE SIMULATION — AI houses with communal rep, résumés (portable
   W/L records), solo-vs-team stakes, the banner-burn liturgy on
   dethronement.
7. STYLE RECIPES — 3-style fighter composition as data (brawl/judo/BJJ/
   capoeira/etc. trait blocks) = the emergence engine of fighters.

## ABSTRACTIONS (Phase A honesty)
- Top-down / block-out arena, capsule fighters, colored limb overlays
  (no anatomical ragdoll, no morphs).
- Grabs/submissions = staged prompts with positional contests, not
  full anim trees.
- Drowning = stamina drain + posture states in the kill-zone, not
  fluid sim.

## ENGINE-WORK ITEMS (delivered to the engine BY the game — the Rage pattern)
- Hitman3D's sample-project gap (per HITMAN3D_STATUS.md) = filled by
  this pilot: `PitProtocol` template/Gem, project pre-enabled.
- Director gem's first production customer: pit entities, arena build,
  spawn/wave scripting driven through Director actions (NL + LLM later;
  keyword parser sufficient for Phase A).
- AssetHub's first in-engine import client for arena props.

## BUILD PLAN (blocks of work, engine-gated by the WSL build campaign)
1. `PitProtocol` gem skeleton (game + client targets; no editor panel).
2. Bout state machine (no rounds; stamina; limb states; bite/drown/walk-out).
3. Fighter recipe component (3-style data table + combat moveset mapping).
4. House AI + résumé subsystem + banner-burn event flow (log first,
   ceremony staging later).
5. Block-out arena scene + playable 1v1 vs AI house fighter.
6. Playtest gate: does the DESIGN hold? (arc, decisions, drama) —
   then Phase B or doctrine revision.

## STUDIO NOTES
- The Stride skeleton (grab/guard/punch anims) = mechanics reference.
- WSL box = headless build/test environment (see AGENTS.md; the no-sudo
  userland procedure from campaign 1b-1d = future AGENTS.md section).
- Windows machine (C:\JSS\hitman3d-engine) = editor development home.