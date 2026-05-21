#pragma once

#include <Hitman3D/AssetHub/AssetHubHttpTypes.h>

namespace Hitman3D::AssetHub
{
    //! Lightweight HTTP GET used by AssetHub marketplace adapters.
    //! O3DE removed the HttpRequestor gem (and AWSNativeSDKInit); AssetHub carries its own client.
    class AssetHubHttpClient
    {
    public:
        static void GetAsync(
            const AZStd::string& url,
            const HttpHeaders& headers,
            HttpTextCallback onDone);

        static bool GetBlocking(
            const AZStd::string& url,
            const HttpHeaders& headers,
            AZStd::string& outBody,
            HttpResponseCode& outCode,
            AZStd::string& outError);
    };
}
