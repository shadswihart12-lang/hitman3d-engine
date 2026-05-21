// ═══════════════════════════════════════════════════════════════
// IntentParser.cpp
// Keyword-based NL parser. Designed to be replaced by an LLM
// backend in a future release — the ParsedIntent contract stays
// the same regardless of the parsing strategy.
// ═══════════════════════════════════════════════════════════════

#include "IntentParser.h"

#include <AzCore/std/string/conversions.h>
#include <cstdlib>
#include <cstring>

namespace Hitman3D::Director
{
    AZStd::string IntentParser::ToLower(const AZStd::string& s)
    {
        AZStd::string out = s;
        AZStd::to_lower(out.begin(), out.end());
        return out;
    }

    bool IntentParser::Contains(const AZStd::string& haystack, const char* needle)
    {
        return haystack.find(needle) != AZStd::string::npos;
    }

    AZStd::string IntentParser::ExtractQuoted(const AZStd::string& input)
    {
        auto q1 = input.find('"');
        if (q1 == AZStd::string::npos)
        {
            q1 = input.find('\'');
        }
        if (q1 == AZStd::string::npos)
        {
            return {};
        }
        char quote = input[q1];
        auto q2 = input.find(quote, q1 + 1);
        if (q2 == AZStd::string::npos)
        {
            return {};
        }
        return input.substr(q1 + 1, q2 - q1 - 1);
    }

    float IntentParser::ExtractNumber(const AZStd::string& input, const char* keyword, float fallback)
    {
        auto pos = input.find(keyword);
        if (pos == AZStd::string::npos)
        {
            return fallback;
        }
        pos += strlen(keyword);
        while (pos < input.size() && (input[pos] == ' ' || input[pos] == '='))
        {
            ++pos;
        }
        if (pos >= input.size())
        {
            return fallback;
        }
        return static_cast<float>(atof(input.c_str() + pos));
    }

    AZ::Vector3 IntentParser::ExtractPosition(const AZStd::string& input)
    {
        float x = ExtractNumber(input, "x", 0.f);
        float y = ExtractNumber(input, "y", 0.f);
        float z = ExtractNumber(input, "z", 0.f);

        // Also try "at X Y Z" pattern
        auto atPos = input.find("at ");
        if (atPos != AZStd::string::npos)
        {
            const char* p = input.c_str() + atPos + 3;
            char* end = nullptr;
            float v1 = strtof(p, &end);
            if (end != p)
            {
                x = v1;
                p = end;
                float v2 = strtof(p, &end);
                if (end != p)
                {
                    y = v2;
                    p = end;
                    float v3 = strtof(p, &end);
                    if (end != p)
                    {
                        z = v3;
                    }
                }
            }
        }
        return AZ::Vector3(x, y, z);
    }

    ParsedIntent IntentParser::Parse(const AZStd::string& input)
    {
        ParsedIntent intent;
        intent.rawInput = input;

        AZStd::string low = ToLower(input);

        // ── Undo / Redo ──────────────────────────────────────
        if (Contains(low, "undo"))
        {
            intent.kind = IntentKind::Undo;
            return intent;
        }
        if (Contains(low, "redo"))
        {
            intent.kind = IntentKind::Redo;
            return intent;
        }

        // ── Help ─────────────────────────────────────────────
        if (Contains(low, "help") || Contains(low, "what can you do"))
        {
            intent.kind = IntentKind::Help;
            return intent;
        }

        // ── Level management ─────────────────────────────────
        if (Contains(low, "create") && Contains(low, "level"))
        {
            intent.kind = IntentKind::CreateLevel;
            intent.entityName = ExtractQuoted(input);
            if (intent.entityName.empty())
            {
                intent.entityName = "NewLevel";
            }
            return intent;
        }
        if (Contains(low, "save"))
        {
            intent.kind = IntentKind::SaveLevel;
            return intent;
        }

        // ── Delete entity ────────────────────────────────────
        if (Contains(low, "delete") || Contains(low, "remove") || Contains(low, "destroy"))
        {
            intent.kind = IntentKind::DeleteEntity;
            intent.entityName = ExtractQuoted(input);
            return intent;
        }

        // ── Move entity ──────────────────────────────────────
        if (Contains(low, "move") || Contains(low, "teleport") || Contains(low, "position"))
        {
            intent.kind = IntentKind::MoveEntity;
            intent.entityName = ExtractQuoted(input);
            intent.position = ExtractPosition(low);
            return intent;
        }

        // ── Scale entity ─────────────────────────────────────
        if (Contains(low, "scale") || Contains(low, "resize"))
        {
            intent.kind = IntentKind::ScaleEntity;
            intent.entityName = ExtractQuoted(input);
            float s = ExtractNumber(low, "scale", 1.f);
            intent.scale = AZ::Vector3(s, s, s);
            return intent;
        }

        // ── Rotate entity ────────────────────────────────────
        if (Contains(low, "rotate") || Contains(low, "spin") || Contains(low, "turn"))
        {
            intent.kind = IntentKind::RotateEntity;
            intent.entityName = ExtractQuoted(input);
            intent.rotation = ExtractPosition(low);
            return intent;
        }

        // ── Terrain ──────────────────────────────────────────
        if (Contains(low, "terrain") && Contains(low, "create"))
        {
            intent.kind = IntentKind::CreateTerrain;
            return intent;
        }
        if (Contains(low, "terrain"))
        {
            intent.kind = IntentKind::ModifyTerrain;
            return intent;
        }

        // ── Skybox ───────────────────────────────────────────
        if (Contains(low, "sky") || Contains(low, "skybox") || Contains(low, "atmosphere"))
        {
            intent.kind = IntentKind::SetSkybox;
            return intent;
        }

        // ── Light ────────────────────────────────────────────
        if (Contains(low, "light") || Contains(low, "lamp") || Contains(low, "sun"))
        {
            intent.kind = IntentKind::PlaceLight;
            intent.position = ExtractPosition(low);
            intent.intensity = ExtractNumber(low, "intensity", 1000.f);
            intent.entityName = ExtractQuoted(input);
            if (intent.entityName.empty())
            {
                intent.entityName = "DirectorLight";
            }
            return intent;
        }

        // ── Script generation ────────────────────────────────
        if (Contains(low, "script") || Contains(low, "behavior") || Contains(low, "when"))
        {
            intent.kind = IntentKind::GenerateScript;
            intent.scriptBody = input;
            return intent;
        }

        // ── Spawn prefab ─────────────────────────────────────
        if (Contains(low, "spawn") || Contains(low, "prefab") || Contains(low, "instantiate"))
        {
            intent.kind = IntentKind::SpawnPrefab;
            intent.prefabPath = ExtractQuoted(input);
            intent.position = ExtractPosition(low);
            return intent;
        }

        // ── Material ─────────────────────────────────────────
        if (Contains(low, "material") || Contains(low, "texture") || Contains(low, "color"))
        {
            intent.kind = IntentKind::ModifyMaterial;
            intent.entityName = ExtractQuoted(input);
            intent.materialName = ExtractQuoted(input);
            return intent;
        }

        // ── Default: create entity ───────────────────────────
        // "add a box", "place a sphere", "create a tree", "make a player"
        if (Contains(low, "add") || Contains(low, "create") || Contains(low, "place") ||
            Contains(low, "make") || Contains(low, "put") || Contains(low, "build"))
        {
            intent.kind = IntentKind::CreateEntity;
            intent.entityName = ExtractQuoted(input);
            if (intent.entityName.empty())
            {
                // Try to pick out a noun after the verb
                for (const char* verb : {"add ", "create ", "place ", "make ", "put ", "build "})
                {
                    auto vpos = low.find(verb);
                    if (vpos != AZStd::string::npos)
                    {
                        AZStd::string rest = input.substr(vpos + strlen(verb));
                        // Skip articles
                        for (const char* article : {"a ", "an ", "the ", "some "})
                        {
                            AZStd::string restLow = ToLower(rest);
                            if (restLow.starts_with(article))
                            {
                                rest = rest.substr(strlen(article));
                            }
                        }
                        // Take until end or next space-delimited keyword
                        auto spacePos = rest.find(" at ");
                        if (spacePos != AZStd::string::npos)
                        {
                            rest = rest.substr(0, spacePos);
                        }
                        intent.entityName = rest;
                        break;
                    }
                }
            }
            intent.position = ExtractPosition(low);
            return intent;
        }

        intent.kind = IntentKind::Unknown;
        return intent;
    }
}
