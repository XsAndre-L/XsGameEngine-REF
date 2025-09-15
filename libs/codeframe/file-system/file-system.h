#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

#include "../string/string.h"

#include "../data/data.h"
#include "../os/os.h"

#include "binary.h"

#include "fs-types.h"

namespace fs = std::filesystem;

// FILE SYSTEM STRUCTS ----------------------------

// FILE OPERATIONS
unsigned int countFiles(const fs::path& directoryPath);

optional<fs::path> fileSearchUpwards(const fs::path& startDir,
    const string& filename);

optional<fs::path> fileSearchDownwards(const fs::path& startDir,
    const string& filename);

void filesRenameRecursive(const fs::path& root, const StringReplaceMap& replaceMap);

// DIRECTORY OPERATIONS
void dirEnsureExists(const fs::path& dirPath);

void dirCopyRecursive(const fs::path& src, const fs::path& dest,
    bool dryRun = false);

void replaceStringRecursive(const fs::path& root, const StringReplaceMap& replaceMap);

// JSON

cf::ordered_json fileToJson(const fs::path& path);
cf::ordered_json directoryToJson(const fs::path& path, bool verbose);
// File System to Json
cf::ordered_json fileSystemToJson(const fs::path& path, bool verbose = false);

// --- Path ---
#pragma region
string fileName(const string& path);
string fileName(const fs::path& path);

string extension(const string& path);
string extension(const fs::path& path);

fs::path pathRelativeTo(const vector<fs::path>& paths,
    const fs::path& basePath = fs::path());
string pathTrimFolders(const string& originalPath, int n);
string pathRemoveExt(const string& fileName);
#pragma endregion

// --- File ---
#pragma region
string readFile(const string& filePath);

bool writeFile(const string& str, const string& filePath, bool dryRun = false,
    bool verbose = false);

void copyFile(const fs::path& src, const string& fileName,
    const fs::path& dest);

string fileFindReplace(const string& filePath, const string& oldString,
    const string& newString);

// FILE VALIDATIONS
bool isTextFile(const fs::path& path);
#pragma endregion
