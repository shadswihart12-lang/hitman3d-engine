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

#include <AzCore/Settings/SettingsRegistry.h>

namespace Hitman3D::AssetHub
{
    namespace
    {
        AZStd::string SettingsKey(const AZStd::string& key)
        {
            return AZStd::string("/Hitman3D/AssetHub/Credentials/") + key;
        }
    }

    bool CredentialStore::Set(const AZStd::string& key, const AZStd::string& value)
    {
        return AZ::SettingsRegistry::Get() && AZ::SettingsRegistry::Get()->Set(SettingsKey(key), value);
    }

    bool CredentialStore::Get(const AZStd::string& key, AZStd::string& outValue)
    {
        return AZ::SettingsRegistry::Get() && AZ::SettingsRegistry::Get()->Get(outValue, SettingsKey(key));
    }

    bool CredentialStore::Clear(const AZStd::string& key)
    {
        return AZ::SettingsRegistry::Get() && AZ::SettingsRegistry::Get()->Remove(SettingsKey(key));
    }
}

#endif
