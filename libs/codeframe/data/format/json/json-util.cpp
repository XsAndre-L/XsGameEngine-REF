#include "json-util.h"

// CREATE
void createJsonFile(const string& path,
    const cf::json& defaultContent = {})
{
    if (fs::exists(path)) {
        console.warn("File already exists: " + path);
        return;
    }

    fs::create_directories(
        fs::path(path).parent_path());
    std::ofstream file(path);

    if (!file) {
        console.error("Failed to create file: " + path);
        return;
    }

    file << defaultContent.dump(4); // Pretty-print with 4-space indent
}

// UPDATE
void updateJsonFile(const fs::path& filePath, const cf::ordered_json& jsonData)
{
    // Create parent directories if they do not exist
    fs::create_directories(filePath.parent_path());

    // Write to a temporary file first
    fs::path tempFilePath = filePath;
    tempFilePath += ".tmp";

    {
        std::ofstream outputStream(tempFilePath);
        if (!outputStream) {
            console.error("Failed to open temporary file for writing: " + tempFilePath.string());
            return;
        }

        outputStream << jsonData.dump(4); // Pretty-print with 4 spaces indentation
        outputStream.flush();
        if (!outputStream) {
            console.error("Failed to write data to temporary file: " + tempFilePath.string());
            return;
        }
        outputStream.close();
    }

    // Atomically replace the old file with the new file
    std::error_code errorCode;
    fs::rename(tempFilePath, filePath, errorCode);
    if (errorCode) {
        console.error("Failed to move temporary file to target: " + errorCode.message());
    }
}

// GET DATA
cf::json getJsonValue(cf::json jsonData, string key)
{
    if (jsonData.find(key) == jsonData.end()) {
        // console.logLn("Failed to find key: " + key);
        return "";
    }

    return jsonData[key];
}

cf::ordered_json getJsonValues(cf::ordered_json& jsonData, vector<string>& keys)
{
    cf::ordered_json fetchedValues = cf::ordered_json::object();

    for (const auto& key : keys) {
        try {
            fetchedValues[key] = jsonData[key];
        } catch (const std::exception& e) {
            console.error("Failed to find config value for key: " + key);
        }
    }

    return fetchedValues;
}

cf::json loadJsonFile(fs::path path)
{
    std::ifstream file(path);

    string filename = fileName(path);
    path = (fs::current_path() / path).generic_string();

    if (!file || file.peek() == std::ifstream::traits_type::eof()) {
        // console.warn("JSON file is missing or empty: " + path.string());
        return cf::json::object(); // Safe fallback
    }

    try {
        cf::json data;
        file >> data;
        return data;
    } catch (const cf::json::parse_error& e) {
        console.error("JSON parse error in '" + filename + "': " + string(e.what()));
        return cf::json::object(); // Safe fallback
    }
}

std::optional<cf::json> readJsonFile(fs::path path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return std::nullopt;
    }

    string jsonString((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    try {
        return cf::json::parse(jsonString);
    } catch (const cf::json::exception&) {
        return std::nullopt;
    }
}

vector<string> getJsonKeys(cf::json& jsonData)
{
    vector<string> keys;
    for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
        keys.push_back(it.key());
    }
    return keys;
}

// bool nodeExists(cf::json& jsonData, const string& targetName) {
//     for (const auto& node : jsonData["nodes"]) {
//         if (node["id"] == targetName) {
//             return true;
//         }
//     }
//     return false;
// }