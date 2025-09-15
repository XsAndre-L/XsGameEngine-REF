#include "file-system.h"

// --- DIRECTORY OPERATIONS ---
#pragma region

void dirEnsureExists(const fs::path& dirPath)
{
    // CHECK IF THE DIRECTORY EXISTS
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        return;
    } else {
        // if (fs::create_directories(dirPath)) {
        //     console.logVerbose("Created Directory: " + dirPath.generic_string());
        // } else {
        //     console.error("Failed To Create Directory: " + dirPath.generic_string());
        // }
        if (!fs::create_directories(dirPath)) {
            // console.logVerbose("Created Directory: " + dirPath.generic_string());
            console.error("Failed To Create Directory: " + dirPath.generic_string());
        }
    }
}

void dirCopyRecursive(const fs::path& src, const fs::path& dest, bool dryRun)
{
    if (!fs::exists(src) || !fs::is_directory(src)) {
        throw std::invalid_argument(
            "Source directory does not exist or is not a directory.");
    }

    if (dryRun) {
        console.logLnM("Copying directory: " + src.string() + " to " + dest.string());
    } else {
        fs::create_directories(dest);
    }

    for (const auto& dirEntry : fs::directory_iterator(src)) {
        const auto& path = dirEntry.path();
        if (fs::is_directory(path)) {
            dirCopyRecursive(path, dest / path.filename(), dryRun);
        } else if (fs::is_regular_file(path)) {
            if (dryRun) // For the sake of Dry Run
            {
                console.logLn("Copying file: " + path.string(), { YELLOW });
                continue;
            }

            fs::copy(path, dest / path.filename());
        } else {
            console.error("Unsupported file type: " + path.string());
        }
    }
}

// Recursively replaces strings in all text files within the given directory
void replaceStringRecursive(const fs::path& root, const StringReplaceMap& replaceMap)
{
    for (auto& entry : fs::recursive_directory_iterator(root)) {
        if (fs::is_regular_file(entry.path()) && isTextFile(entry.path())) {
            // Read file content
            std::ifstream fileIn(entry.path(), std::ios::in | std::ios::binary);
            if (!fileIn)
                continue;

            std::ostringstream buffer;
            buffer << fileIn.rdbuf();
            string content = buffer.str();
            fileIn.close();

            // Replace content
            string newContent = replaceAllInMap(content, replaceMap);

            // Only overwrite if content changed
            if (newContent != content) {
                std::ofstream fileOut(entry.path(), std::ios::out | std::ios::binary | std::ios::trunc);
                fileOut << newContent;
                fileOut.close();
            }
        }
    }
}
#pragma endregion