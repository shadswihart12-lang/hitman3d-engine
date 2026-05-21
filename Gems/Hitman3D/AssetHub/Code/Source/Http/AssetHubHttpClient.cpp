#include <Hitman3D/AssetHub/AssetHubHttpClient.h>

#include <AzCore/Jobs/JobFunction.h>
#include <AzCore/Utils/Utils.h>

#include <cstdio>
#include <memory>

namespace Hitman3D::AssetHub
{
    namespace
    {
        HttpResponseCode MapStatus(int status)
        {
            if (status == 200)
            {
                return HttpResponseCode::Ok;
            }
            if (status == 401)
            {
                return HttpResponseCode::Unauthorized;
            }
            if (status == 404)
            {
                return HttpResponseCode::NotFound;
            }
            if (status == 0)
            {
                return HttpResponseCode::RequestNotMade;
            }
            return HttpResponseCode::Error;
        }

        bool RunCurlGet(
            const AZStd::string& url,
            const HttpHeaders& headers,
            AZStd::string& outBody,
            int& outStatus,
            AZStd::string& outError)
        {
            AZ::IO::FixedMaxPath tempDir;
            if (!AZ::Utils::GetExecutableDirectory(tempDir.Native()))
            {
                outError = "Could not resolve temp directory for HTTP download";
                outStatus = 0;
                return false;
            }

            AZ::IO::FixedMaxPath headerPath = tempDir / "assethub_curl_headers.txt";
            AZ::IO::FixedMaxPath bodyPath = tempDir / "assethub_curl_body.bin";

            {
                FILE* headerFile = fopen(headerPath.c_str(), "wb");
                if (!headerFile)
                {
                    outError = "Failed to create curl header file";
                    outStatus = 0;
                    return false;
                }
                for (const auto& [name, value] : headers)
                {
                    fprintf(headerFile, "%s: %s\n", name.c_str(), value.c_str());
                }
                fclose(headerFile);
            }

            AZStd::string command;
            command = AZStd::string::format(
                "curl -sS -L -w \"%%{http_code}\" -D \"%s\" -o \"%s\"",
                headerPath.c_str(),
                bodyPath.c_str());

            for (const auto& [name, value] : headers)
            {
                command += AZStd::string::format(" -H \"%s: %s\"", name.c_str(), value.c_str());
            }
            command += AZStd::string::format(" \"%s\" 2>/dev/null", url.c_str());

            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe)
            {
                outError = "curl not available — install curl or set PATH";
                outStatus = 0;
                return false;
            }

            char statusBuf[32] = {};
            if (fgets(statusBuf, sizeof(statusBuf), pipe) == nullptr)
            {
                pclose(pipe);
                outError = "curl produced no status code";
                outStatus = 0;
                return false;
            }
            pclose(pipe);

            outStatus = atoi(statusBuf);

            FILE* bodyFile = fopen(bodyPath.c_str(), "rb");
            if (!bodyFile)
            {
                outError = AZStd::string::format("curl failed (HTTP %d)", outStatus);
                return false;
            }

            fseek(bodyFile, 0, SEEK_END);
            long size = ftell(bodyFile);
            fseek(bodyFile, 0, SEEK_SET);
            if (size > 0)
            {
                outBody.resize(static_cast<size_t>(size));
                fread(outBody.data(), 1, static_cast<size_t>(size), bodyFile);
            }
            else
            {
                outBody.clear();
            }
            fclose(bodyFile);

            remove(headerPath.c_str());
            remove(bodyPath.c_str());
            return outStatus >= 200 && outStatus < 300;
        }
    }

    void AssetHubHttpClient::GetAsync(
        const AZStd::string& url,
        const HttpHeaders& headers,
        HttpTextCallback onDone)
    {
        auto* job = AZ::CreateJobFunction(
            [url, headers, onDone = AZStd::move(onDone)]()
            {
                AZStd::string body;
                AZStd::string error;
                int status = 0;
                const bool ok = RunCurlGet(url, headers, body, status, error);
                if (onDone)
                {
                    onDone(
                        ok ? body : error,
                        MapStatus(status));
                }
            },
            true,
            nullptr);
        job->Start();
    }

    bool AssetHubHttpClient::GetBlocking(
        const AZStd::string& url,
        const HttpHeaders& headers,
        AZStd::string& outBody,
        HttpResponseCode& outCode,
        AZStd::string& outError)
    {
        int status = 0;
        const bool ok = RunCurlGet(url, headers, outBody, status, outError);
        outCode = MapStatus(status);
        return ok;
    }
}
