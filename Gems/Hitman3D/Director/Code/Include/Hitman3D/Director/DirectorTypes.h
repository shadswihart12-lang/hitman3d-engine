// ═══════════════════════════════════════════════════════════════
// DirectorTypes.h — shared types for the Director AI system
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/base.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/TypeInfoSimple.h>

namespace Hitman3D::Director
{
    using CommandId = AZ::u64;

    enum class IntentKind : AZ::u8
    {
        Unknown = 0,
        CreateEntity,
        DeleteEntity,
        MoveEntity,
        ScaleEntity,
        RotateEntity,
        CreateTerrain,
        ModifyTerrain,
        SetSkybox,
        SpawnPrefab,
        GenerateScript,
        ModifyMaterial,
        PlaceLight,
        CreateLevel,
        SaveLevel,
        Undo,
        Redo,
        Help
    };

    struct ParsedIntent
    {
        AZ_TYPE_INFO(ParsedIntent, "{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}");

        IntentKind kind = IntentKind::Unknown;
        AZStd::string rawInput;
        AZStd::string entityName;
        AZStd::string prefabPath;
        AZStd::string scriptBody;
        AZStd::string materialName;
        AZ::Vector3 position = AZ::Vector3::CreateZero();
        AZ::Vector3 scale = AZ::Vector3::CreateOne();
        AZ::Vector3 rotation = AZ::Vector3::CreateZero();
        float radius = 1.0f;
        float intensity = 1.0f;
    };

    struct CommandResult
    {
        AZ_TYPE_INFO(CommandResult, "{B2C3D4E5-F6A7-8901-BCDE-F12345678901}");

        bool success = false;
        AZStd::string message;
        AZStd::string detail;
        CommandId commandId = 0;
    };
}
