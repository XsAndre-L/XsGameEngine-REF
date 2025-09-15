#include "cli.h"

// Splits a string by a delimiter
vector<string> getCommand()
{
    string command;
    console.log("> ", { BLUE });
    getline(std::cin, command);

    return splitString(command, ' ');
}

ParsedCommand parseCommand(vector<string>& args)
{
    ParsedCommand result;
    result.cmd = toLowerCase(args[0]);
    args.erase(args.begin());

    if (args.empty()) {
        result.args.push_back("");
        return result;
    }

    for (const auto& arg : args) {
        result.args.push_back(arg);
    }

    return result;
};

// Help Command
void helpMenu(const vector<CommandDefinition>& commands,
    const optional<rgb>& primary, const optional<rgb>& secondary)
{
    console.logHeader(" HELP ");
    console.logLn();

    for (size_t i = 0; i < commands.size(); i++) {
        const CommandDefinition current = commands[i];

        if (current.info.cmd.empty()) // Skip Over Empty Spots in The Array
            continue;

        console.log(current.info.cmd + " | ", { MAGENTA });
        string aliases;
        for (const auto& alias : current.info.aliases) {
            aliases += alias + " ";
        }
        console.logLn(aliases);
        console.log(current.info.name + ": " + current.info.description, { GRAY });

        console.logLn();
        console.logLn();
    }
}

// bool executeCommand(vector<string> &args, bool silent) // need the silent
// flag for when we run the 'run' command  and call execute command in there
// {
//   if (args.empty())
//     return CONTINUE;

//   const vector<string> allArgs = args;

//   ParsedCommand cmd_section = parseCommand(args);

//   // Check base functions
//   if (cmd_section.cmd == "exit")
//     return EXIT;

//   if (cmd_section.cmd == "help")
//   {
//     help_cmd_func();
//     return CONTINUE;
//   }

//   if (cmd_section.cmd == "info")
//   {
//     printLogo();
//     console.logLn("Process ID: " + getProcessID());
//     return CONTINUE;
//   }

//   // if (cmd_section.cmd == "setup_dependencies")
//   // {
//   //   downloadDependencies();
//   // }

//   for (const auto &command : commands)
//   {
//     // Check if input match command
//     if (cmd_section.cmd == command.info.cmd)
//     {
//       command.action(cmd_section.args);
//       return CONTINUE;
//     }

//     for (const auto &alias : command.info.aliases)
//     {
//       if (cmd_section.cmd == alias)
//       {
//         command.action(cmd_section.args);
//         return CONTINUE;
//       }
//     }
//   }

//   if (silent)
//   { // we dont want to log an error
//     return EXIT;
//   }

//   // run_cmd.action(allArgs);
//   return CONTINUE;
// }
