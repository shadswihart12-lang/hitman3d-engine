// ═══════════════════════════════════════════════════════════════
// CredentialStore_Windows.cpp
// Windows Credential Manager backend for token storage.
// ═══════════════════════════════════════════════════════════════

#include "CredentialStore.h"

#if defined(AZ_PLATFORM_WINDOWS)

#include <AzCore/PlatformIncl.h>
#include <wincred.h>
#pragma comment(lib, "advapi32.lib")

namespace Hitman3D::AssetHub
{
    bool CredentialStore::Set(const AZStd::string& key, const AZStd::string& value)
    {
        CREDENTIALA cred = {};
        cred.Type = CRED_TYPE_GENERIC;
        cred.TargetName = const_cast<char*>(key.c_str());
        cred.CredentialBlobSize = static_cast<DWORD>(value.size());
        cred.CredentialBlob = (LPBYTE)value.data();
        cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
        return ::CredWriteA(&cred, 0) == TRUE;
    }

    bool CredentialStore::Get(const AZStd::string& key, AZStd::string& outValue)
    {
        PCREDENTIALA cred = nullptr;
        if (!::CredReadA(key.c_str(), CRED_TYPE_GENERIC, 0, &cred))
            return false;
        outValue.assign(reinterpret_cast<const char*>(cred->CredentialBlob),
                        cred->CredentialBlobSize);
        ::CredFree(cred);
        return true;
    }

    bool CredentialStore::Clear(const AZStd::string& key)
    {
        ::CredDeleteA(key.c_str(), CRED_TYPE_GENERIC, 0);
        return true;  // idempotent
    }
}

#else

// Non-Windows stub — to be replaced with Keychain / libsecret backends.
namespace Hitman3D::AssetHub
{
    bool CredentialStore::Set(const AZStd::string&, const AZStd::string&)   { return false; }
    bool CredentialStore::Get(const AZStd::string&, AZStd::string&)         { return false; }
    bool CredentialStore::Clear(const AZStd::string&)                       { return true;  }
}

#endif
