// ═══════════════════════════════════════════════════════════════
// IntentParser.h — Turns natural-language input into ParsedIntent
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <Hitman3D/Director/DirectorTypes.h>

namespace Hitman3D::Director
{
    class IntentParser final
    {
    public:
        //! Parse a raw user string into a structured intent.
        //! Uses keyword matching now; future versions will call an LLM backend.
        static ParsedIntent Parse(const AZStd::string& input);

    private:
        static AZStd::string ToLower(const AZStd::string& s);
        static bool Contains(const AZStd::string& haystack, const char* needle);
        static AZ::Vector3 ExtractPosition(const AZStd::string& input);
        static AZStd::string ExtractQuoted(const AZStd::string& input);
        static float ExtractNumber(const AZStd::string& input, const char* keyword, float fallback);
    };
}
