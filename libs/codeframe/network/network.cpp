#include "httplib/httplib.h"

#include "network.h"

std::optional<string> downloadFile(
    const std::optional<std::string> domainUrl,
    const string& endpoint,
    string& errorMsg)
{
    if (!domainUrl) {
        errorMsg = "SERVER_DOMAIN is not set";
        return std::nullopt;
    }

    httplib::Client cli(domainUrl->c_str());
    auto res = cli.Get(endpoint);

    if (!res || res->status != 200) {
        errorMsg = "Download Failed: ";
        if (!res) {
            errorMsg += "no response";
        } else if (!res->body.empty()) {
            errorMsg += res->body;
        } else {
            errorMsg += std::to_string(res->status);
        }
        return std::nullopt;
    }

    return res->body; // Works for ANY file type (binary or text)
}