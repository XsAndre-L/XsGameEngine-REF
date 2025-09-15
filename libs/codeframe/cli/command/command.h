#pragma once

#include <array>
#include <core/base.h>
#include <functional>
#include <set>

#include "command-builder.h"

struct CommandInfo {
    string cmd;
    string name;
    string description;
    std::array<string, 3> aliases;
    const std::set<string> p_args;
};

struct CommandDefinition {
    std::function<void(const vector<string>& args)> action;
    const CommandInfo info;
    const std::set<string> np_args;
};