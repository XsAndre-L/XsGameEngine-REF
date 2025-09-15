#include "file-system.h"

// --- FILE INFORMATION ---
#pragma region
string extension(const string& path)
{
    fs::path p(path);
    return p.extension().string();
}
string extension(const fs::path& path)
{
    return path.extension().string();
}

string fileName(const string& path)
{
    return fs::path(path).filename().string();
}
string fileName(const fs::path& path)
{
    return path.filename().string();
}
#pragma endregion

// --- PATH MANIPULATION ---
#pragma region

string pathTrimFolders(const string& originalPath, int n)
{
    std::size_t pos = 0;
    for (int i = 0; i < n; ++i) {
        pos = originalPath.find_first_of("\\/", pos);
        if (pos == string::npos) {
            return originalPath; // Return original if n is greater than existing
                                 // directories
        }
        ++pos; // Skip the found backslash
    }
    return originalPath.substr(pos);
}

fs::path pathRelativeTo(const vector<fs::path>& paths,
    const fs::path& basePath)
{
    fs::path dirPath;

    for (size_t i = 0; i < paths.size(); i++) {
        if (i != paths.size() - 1) {
            if (!fs::is_directory(paths[i])) {
                dirPath += fs::path(paths[i]).parent_path();
                string dirPathStr = dirPath.string();
                if (dirPathStr.back() != '/') {
                    dirPathStr += '/';
                }
                dirPath = fs::path(dirPathStr);
                continue;
            }
            dirPath += paths[i];
        } else {
            dirPath += paths[i];
        }
    }

    fs::path canonicalPath = fs::canonical(dirPath);

    fs::path baseDir;
    if (basePath.empty())
        baseDir = fs::current_path();
    else
        baseDir = basePath;

    return fs::relative(canonicalPath, basePath);
}

string pathRemoveExt(const string& fileName)
{
    std::size_t lastDot = fileName.find_last_of(".");
    if (lastDot != string::npos) {
        return fileName.substr(0, lastDot);
    }
    return fileName;
}

#pragma endregion
