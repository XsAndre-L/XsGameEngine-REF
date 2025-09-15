#pragma once

// #include "xhttp/xhttp.h"

#include <optional>

#include "../core/base.h"

std::optional<string> downloadFile(const std::optional<std::string> domainUrl, const string& endpoint, string& errorMsg);