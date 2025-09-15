#pragma once

#include <filesystem>
#include <map>

namespace fs = std::filesystem;

#include "../../core/base.h"
#include "../../string/string.h"

class CommandBuilder {
public:
    CommandBuilder() = default;
    ~CommandBuilder() = default;

    // Command
    const string& getCmd();
    void setCmd(string& cmd);

    // Current Working Directory
    const string& getCwd();
    void setCwd(string& cwd);

    void addSegment(const string& segment);
    void addPath(const fs::path& path);
    void addFlag(const string& flag);
    void addKeyValue(const string& key, const string& value);

    void addRelativeResource(const string& value);

private:
    enum class CommandTokenType {
        Segment,
        Path,
        Flag,
        KeyValue,
        RelativeResource
    };

    struct CommandToken {
        CommandTokenType type;
        string key;
        string value;
    };

    // Command to be executed
    string cmd { "" };
    bool dirty = true;

    // Current working directory, all relative links wil be relative to this
    string cwd { "" };

    vector<CommandToken> tokens;

    string build() const;
};