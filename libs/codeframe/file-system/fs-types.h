#include "../core/base.h"

#include <optional>
#include <string>
#include <vector>

// 1. DownloadActionType enum
enum class DownloadActionType {
    Unzip,
    Delete,
    // Move, // Uncomment if needed
    // Custom // Not supported in C++ like TS function types
};

// 2. DownloadAction struct
struct DownloadAction {
    DownloadActionType type;
    // For 'move', you’d add: std::optional<std::string> destination;
    // For 'custom', not easily represented in C++
};

// 3. ProgressType struct
struct ProgressType {
    int step;
    int completed;
};

// 4. IFile struct
struct File {
    string fileName;
    string type;
    string content;
};

// 5. IDownload struct
struct Download {
    string url;
    vector<DownloadAction> actions;
    // actions is optional in TS; in C++ just leave it empty if none
};

// // 6. IDirectory/children struct
// struct DirectoryChildren {
//     vector<File> files;
//     vector<Directory> directories;
//     vector<Download> downloads;
// };

// struct Directory {
//     string dirName;
//     std::optional<DirectoryChildren> children; // optional like TS
// };

// struct Directory {
//     string name;
//     vector<File> files;
//     vector<Directory> directories;
// };