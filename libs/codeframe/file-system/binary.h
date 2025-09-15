#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../cli/cli.h"
#include "../core/base.h"

namespace fs = std::filesystem;

static const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789+/";

static const std::unordered_set<string> binaryExtensions = {
    ".exe", ".png", ".jpg", ".qch", ".inv", ".ico",
    ".in", ".dll", ".zip", ".pyd", ".dat", ".vsmacros", ".res"
};

// bool isBinaryFile(const string& filePath);
bool isBinary(const string& extension);

vector<char> readBinaryFile(const string& filePath);
bool writeBinaryFile(const fs::path& filePath, const string& data);

string binaryToBase64(const vector<char>& binaryData);