#include "file-system.h"

#pragma region // FILE OPERATIONS

unsigned int countFiles(const fs::path& directoryPath)
{
    unsigned int countFiles = 0;

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry)) {
            ++countFiles;
        }
    }

    return countFiles;
}

std::optional<fs::path> fileSearchUpwards(const fs::path& startDir,
    const string& filename)
{
    fs::path currentDir = startDir;

    while (true) {
        fs::path potentialFile = currentDir / filename;

        if (fs::exists(potentialFile) && fs::is_regular_file(potentialFile)) {
            return potentialFile;
        }

        if (!currentDir.has_parent_path() || currentDir == currentDir.parent_path()) {
            console.error("Could not find file: " + filename + " in current or parent directory.");
            break;
        } else if (currentDir.has_parent_path()) {
            // console.logLn(currentDir.string());
            currentDir = currentDir.parent_path();
        }
    }

    return {};
}

std::optional<fs::path> fileSearchDownwards(const fs::path& startDir,
    const string& filename)
{
    if (!fs::exists(startDir) || !fs::is_directory(startDir)) {
        console.error("Invalid start directory: " + startDir.string());
        return {};
    }

    for (const auto& entry : fs::recursive_directory_iterator(startDir)) {
        if (fs::is_regular_file(entry) && entry.path().filename() == filename) {
            return entry.path(); // Found the file, return the full path
        }
    }

    console.error("Could not find file: " + filename + " in " + startDir.string() + " or its subdirectories.");
    return {};
}

void forEachFileRecursive(
    const fs::path& root,
    std::function<void(const fs::directory_entry&)> callback,
    bool skipOnPermissionDenied = true)
{
    auto options = skipOnPermissionDenied
        ? fs::directory_options::skip_permission_denied
        : fs::directory_options::none;
    for (const auto& entry : fs::recursive_directory_iterator(root, options)) {
        if (fs::is_regular_file(entry.path())) {
            callback(entry);
        }
    }
}

void filesRenameRecursive(const fs::path& root, const StringReplaceMap& replaceMap)
{
    forEachFileRecursive(root, [&](const fs::directory_entry& entry) {
        fs::path oldPath = entry.path();
        fs::path parent = oldPath.parent_path();
        std::string fileName = oldPath.filename().string();
        std::string newFileName = fileName;

        // Replace in file name
        for (const auto& [from, to] : replaceMap) {
            size_t pos = 0;
            while ((pos = newFileName.find(from, pos)) != std::string::npos) {
                newFileName.replace(pos, from.length(), to);
                pos += to.length();
            }
        }
        if (newFileName != fileName) {
            fs::path newPath = parent / newFileName;
            fs::rename(oldPath, newPath);
        }
    });
}
#pragma endregion

// --- JSON ---
#pragma region

cf::json processFile(const fs::path& path)
{
    const string extension = path.extension().string();
    cf::json fileJson;
    fileJson["fileName"] = path.filename().string();
    fileJson["type"] = path.extension().string();

    if (isBinary(extension)) {
        vector<char> binaryData = readBinaryFile(path.string()); // Implement readBinaryFile
        fileJson["content"] = binaryToBase64(binaryData);
    } else {
        fileJson["content"] = readFile(path.string());
    }

    return fileJson;
}

cf::ordered_json fileToJson(const fs::path& path)
{

    cf::ordered_json fileJson;
    fileJson["fileName"] = path.filename().string();
    fileJson["type"] = path.extension().string();

    const string extension = path.extension().string();

    string fileContent;

    try {
        if (isBinary(extension)) {
            vector<char> binaryData = readBinaryFile(path.string());
            fileContent = binaryToBase64(binaryData);
        } else {
            fileContent = readFile(path.string());
        }

        fileJson["content"] = fileContent;
    } catch (const std::exception& e) {
        console.log(path.string());
        std::cerr << e.what() << '\n';
    }

    return fileJson;
}

cf::ordered_json directoryToJson(const fs::path& path, bool verbose)
{
    cf::ordered_json dirJson;
    dirJson["dirName"] = path.filename().string();

    cf::ordered_json dirChildren;
    dirChildren["files"] = cf::json::array();
    dirChildren["donwloads"] = cf::json::array();
    dirChildren["directories"] = cf::json::array();

    for (const auto& dirEntry : fs::directory_iterator(path)) {
        const auto& entryPath = dirEntry.path();

        if (fs::is_directory(entryPath)) {
            dirChildren["directories"].push_back(directoryToJson(entryPath, verbose));
        } else if (fs::is_regular_file(entryPath)) {
            dirChildren["files"].push_back(fileToJson(entryPath));
        } else {
            // if you want to handle downloads, detect & push here:
            // dirChildren["downloads"].push_back(downloadToJson(entryPath));
            console.error("Unsupported file type: " + entryPath.string());
        }
    }

    dirJson["children"] = dirChildren;

    return dirJson;
}

cf::ordered_json fileSystemToJson(const fs::path& path, bool verbose)
{
    try {
        if (!fs::exists(path)) {
            console.error("File or directory does not exist: " + path.string());
            return cf::ordered_json();
        }

        if (fs::is_directory(path)) {
            return directoryToJson(path, verbose);
        }

        if (fs::is_regular_file(path)) {
            return fileToJson(path);
        }

        console.error("Unsupported file type: " + path.string());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        console.error("JSON conversion failed — likely due to invalid UTF-8 "
                      "content in one or more files.");
    }

    return cf::ordered_json();
}

#pragma endregion