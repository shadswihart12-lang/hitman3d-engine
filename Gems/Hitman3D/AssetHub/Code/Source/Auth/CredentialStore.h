// ═══════════════════════════════════════════════════════════════
// CredentialStore.h
// Cross-platform shim over OS-level credential storage.
// Windows: Windows Credential Manager (CredWrite / CredRead).
// macOS/Linux: TODO — keychain / libsecret.
// Part of: Hitman 3d Asset Hub
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/std/string/string.h>

namespace Hitman3D::AssetHub
{
    class CredentialStore
    {
    public:
        //! Store a credential under a key. Returns true on success.
        static bool Set(const AZStd::string& key, const AZStd::string& value);
        //! Retrieve a credential. Returns false if not present.
        static bool Get(const AZStd::string& key, AZStd::string& outValue);
        //! Remove a credential. Idempotent — succeeds even if not present.
        static bool Clear(const AZStd::string& key);
    };
}
