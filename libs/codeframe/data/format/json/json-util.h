#pragma once

#include <fstream>
#include <optional>

#include "json.h"

#include "../../../cli/cli.h"
#include "../../../core/base.h"
#include "../../../file-system/file-system.h"

namespace fs = std::filesystem;

// CREATE
void createJsonFile(const string& path, const cf::json& defaultContent);

// UPDATE
void updateJsonFile(const fs::path& filePath, const cf::ordered_json& jsonData);

// GET
cf::json getJsonValue(cf::json jsonData, string key);

cf::ordered_json getJsonValues(cf::ordered_json& jsonData, vector<string>& keys);

cf::json loadJsonFile(fs::path path);
std::optional<cf::json> readJsonFile(fs::path path);

vector<string> getJsonKeys(cf::json& jsonData);

// Adds a string value to a JSON object only if it exists and is not empty
template <typename T>
void setJsonItem(cf::ordered_json& json, const std::string& key, const std::optional<T>& value)
{
    if (value && !value->empty())
        json[key] = *value;
}