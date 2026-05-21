#pragma once

#include <AzCore/std/containers/map.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>

namespace Hitman3D::AssetHub
{
    enum class HttpMethod
    {
        Get,
        Post
    };

    enum class HttpResponseCode
    {
        RequestNotMade = 0,
        Ok = 200,
        Unauthorized = 401,
        NotFound = 404,
        Error = 500
    };

    using HttpHeaders = AZStd::map<AZStd::string, AZStd::string>;
    using HttpTextCallback = AZStd::function<void(const AZStd::string& body, HttpResponseCode code)>;
}
