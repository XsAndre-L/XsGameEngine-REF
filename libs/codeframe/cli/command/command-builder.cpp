#include "command-builder.h"

#include <filesystem>
#include <sstream>

const string& CommandBuilder::getCmd()
{
    if (dirty) {
        cmd = build();
        dirty = false;
    }
    return cmd;
};
void CommandBuilder::setCmd(string& cmd)
{
    this->cmd = cmd;
};

const string& CommandBuilder::getCwd()
{
    return cwd;
};
void CommandBuilder::setCwd(string& cwd)
{
    this->cwd = cwd;
};

void CommandBuilder::addSegment(const string& segment)
{
    tokens.push_back({ CommandTokenType::Segment, "", segment });
    dirty = true;
};
void CommandBuilder::addPath(const fs::path& path)
{
    tokens.push_back({ CommandTokenType::Path, "", path.generic_string() });
    dirty = true;
};
void CommandBuilder::addFlag(const string& flag)
{
    tokens.push_back({ CommandTokenType::Flag, "", flag });
    dirty = true;
};
void CommandBuilder::addKeyValue(const string& key, const string& value)
{
    tokens.push_back({ CommandTokenType::KeyValue, key, value });
    dirty = true;
};

void CommandBuilder::addRelativeResource(const string& value)
{
    tokens.push_back({ CommandTokenType::RelativeResource, "", value });
    dirty = true;
};

string CommandBuilder::build() const
{
    std::ostringstream oss;
    oss << cmd;
    for (const auto& token : tokens) {
        switch (token.type) {
        case CommandTokenType::Flag:
            oss << " -" << token.value;
            break;
        case CommandTokenType::Path:
            oss << " \"" << token.value << "\"";
            break;
        case CommandTokenType::Segment:
            oss << " " << token.value;
            break;
        case CommandTokenType::RelativeResource: {
            std::filesystem::path fullPath = std::filesystem::path(cwd) / token.value;
            oss << " \"" << fullPath.generic_string() << "\"";
            break;
        }
        case CommandTokenType::KeyValue:
            oss << " -" << token.key << "=" << token.value;
            break;
        }
    }
    return trim(oss.str());
}