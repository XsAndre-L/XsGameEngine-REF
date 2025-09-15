#include "file-system.h"

// #include <unordered_set>

// static string read(const fs::path& path)
// {
//     std::ifstream fileIn(path);
//     if (!fileIn.is_open()) {
//         console.error("File cannot be opened!");
//         return "";
//     }
//     // Read entire file content
//     std::stringstream buffer;
//     buffer << fileIn.rdbuf();
//     return buffer.str();
// }

// static vector<char> readBinary(const string& path)
// {
//     std::ifstream file(path, std::ios::in | std::ios::binary);
//     if (!file)
//         return {};
//     // Seek to end to get size
//     file.seekg(0, std::ios::end);
//     size_t size = file.tellg();
//     file.seekg(0, std::ios::beg);

//     vector<char> data(size);
//     file.read(data.data(), size);
//     return data;
// }

string readFile(const string& filePath)
{
    std::ifstream fileIn(filePath);
    if (!fileIn.is_open()) {
        console.error("File cannot be opened!");
        return "";
    }
    // Read entire file content
    std::stringstream buffer;
    buffer << fileIn.rdbuf();
    return buffer.str();
}

bool writeFile(const string& str, const string& filePath, bool dryRun,
    bool verbose)
{
    if (dryRun) {
        // Log what would be done, but don't actually do anything
        console.logLn("Would write string to file: " + filePath, { YELLOW });
        return true; // Indicate success
    }

    // Create directories
    fs::path path(filePath);
    if (verbose)
        console.logVerbose("Creating directories: " + path.parent_path().string());

    fs::create_directories(path.parent_path());

    // Open the file in output mode, will overwrite existing content
    std::ofstream outFile(filePath);
    if (!outFile) {
        console.error("Unable to open file for writing: " + filePath);
        return false;
    }
    // Write the string to the file
    outFile << str;
    // Close the file
    outFile.close();

    return true;
}

void copyFile(const fs::path& src, const string& fileName,
    const fs::path& dest)
{
    try {
        // Iterate through the source directory
        for (const auto& entry : fs::directory_iterator(src)) {
            // Check if the current entry is a file and matches the fileName without
            // considering the extension
            if (entry.is_regular_file() && entry.path().stem() == fileName) {
                // Construct the destination path
                fs::path destPath = dest / entry.path().filename();
                // Copy the file
                fs::copy(entry.path(), destPath, fs::copy_options::recursive);
                std::cout << "Copied: " << entry.path() << " to " << destPath
                          << std::endl;
                return; // File found and copied, exit the function
            }
        }
    } catch (const std::exception& e) {
        console.error("Error while copying resource: " + string(e.what()));
        return;
    }
}

string fileFindReplace(const string& filePath,
    const string& oldString,
    const string& newString)
{
    string fileContent = readFile(filePath);

    size_t pos = fileContent.find(oldString);
    while (pos != string::npos) {
        fileContent.replace(pos, oldString.size(), newString);
        pos = fileContent.find(oldString, pos + newString.size());
    }

    return fileContent;
}

#pragma region // FILE VALIDATION

// Returns true if path has a known text file extension
bool isTextFile(const fs::path& path)
{
    static const std::unordered_set<std::string> textExts = {
        ".c", ".cpp", ".h", ".hpp", ".c", ".txt", ".md", ".json", ".codeframe"
    };

    auto ext = path.extension().string();

    // Handle dotfiles like ".codeframe"
    if (ext.empty() && path.filename().string().starts_with(".codeframe"))
        return true;

    return textExts.count(ext) > 0;
}

#pragma endregion
