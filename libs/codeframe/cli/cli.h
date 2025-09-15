#pragma once

#include <sstream>
#include <string>
#include <vector>

// #include "args/args.h"
#include "command/command.h"
#include "console/console.h"

struct ParsedCommand {
    string cmd;
    vector<string> args;
};

vector<string> getCommand();
ParsedCommand parseCommand(vector<string>& args);

// MENUS

void helpMenu(const vector<CommandDefinition>& commands,
    const optional<rgb>& primary = MAGENTA,
    const optional<rgb>& secondary = std::nullopt);
