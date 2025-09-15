#include "console.h"

#ifdef _WIN32
#include <windows.h>
struct _Utf8Out {
    _Utf8Out() { SetConsoleOutputCP(CP_UTF8); }
} _utf8_out;
#endif

// constexpr char FILL[] = "\xE2\x96\x93"; // ▓ (U+2593)
constexpr char LEFT[] = "\xE2\x96\x90"; // ▐
constexpr char RIGHT[] = "\xE2\x96\x8C"; // ▌
constexpr char FILL[] = "\xE2\x96\x88"; // █ (U+2588)
constexpr char EMPTY[] = "\xE2\x96\x91"; // ░ (U+2591)

void append_repeat(std::string& out, const char* bytes, std::size_t count)
{
    out.reserve(out.size() + count * 3); // each glyph = 3 UTF-8 bytes
    for (std::size_t i = 0; i < count; ++i)
        out.append(bytes, 3);
}

Console console;

// ─────────────────────────────────────────────────────────────
// Basic Logging - Styled terminal output utilities
//
// Description:    Provides core styled output methods for stdout/stderr.
// Usage:          Use these for various log levels and stream targets.
// Relationships:  Core implementation shared with verbose and header outputs.
// ─────────────────────────────────────────────────────────────

#pragma region // Console Utilities
void Console::hideCursor() // Hide cursor
{
    std::cout << "\x1b[?25l";
}
void Console::showCursor() // Show cursor again
{
    std::cout << "\x1b[?25h";
}

void Console::moveCursorUp(const size_t& lines) // Move cursor up N lines to redraw over old menu
{
    std::cout << "\x1b[" << lines << "A";
}

void Console::clearCursor() // Clear the line before rewriting (optional but nice)
{
    std::cout << "\r\033[2K";
}

void Console::clear()
{
#ifdef _WIN32
    // Try VT (ANSI escape) mode first
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &mode)) {
        // Enable VT mode if possible
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        std::cout << "\x1b[2J\x1b[H"; // ANSI clear + home
        return;
    }

    // Fallback to Win32 console buffer API
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        DWORD count;
        COORD home = { 0, 0 };
        DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(h, ' ', cells, home, &count);
        FillConsoleOutputAttribute(h, csbi.wAttributes, cells, home, &count);
        SetConsoleCursorPosition(h, home);
    }
#else
    // On Linux/macOS: ANSI just works
    std::cout << "\x1b[2J\x1b[H";
#endif
}

void Console::startLoadBar(std::size_t total, const string& label, const string& log)
{
    // const int innerWidth = 40;
    loadBar.label = label;
    loadBar.total = total;
    loadBar.done = 0;
    loadBar.active = true;
    loadBar.hasSeperator = false;
    hideCursor();
    updateLoadBar(0, log);
}

void Console::updateLoadBar(std::size_t delta, const string& log)
{
    // <█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░>
    // (█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░)
    // [█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░]
    // {█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░}
    // ▐█░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▌

    if (!loadBar.active)
        return;
    if (delta == 0)
        delta = 1; // auto-increment by 1
    loadBar.done = std::min(loadBar.done + delta, loadBar.total);

    const int innerWidth = 40;
    double ratio = loadBar.total ? double(loadBar.done) / loadBar.total : 1.0;
    if (ratio > 1.0)
        ratio = 1.0;
    int filled = int(ratio * innerWidth + 0.5);

    string filledBar, emptyBar;
    append_repeat(filledBar, FILL, filled);
    append_repeat(emptyBar, EMPTY, innerWidth - filled);

    // 1) If we have a message, wipe the bar and print it above
    if (!log.empty()) {
        clearCursor();
        if (loadBar.hasSeperator) {
            // clear the existing separator line above the bar
            std::cout << "\x1b[1A\r\x1b[2K";
        }
        logLn(log);

        // print a single separator exactly as wide as the bar body
        std::string sep;
        append_repeat(sep, "—", innerWidth + 2); // '▐' + innerWidth + '▌'
        logLn(sep, { GREEN });

        loadBar.hasSeperator = true;
    }

    std::cout << "\r\033[2K"; // ensure clean line
    if (!loadBar.label.empty())
        std::cout << loadBar.label << ' ';

    // ▐ (ORCHID_DARK)  █ (ORCHID)  ░ (ORCHID_DARK)  ▌ (ORCHID_DARK)
    std::cout << tColor(ORCHID_DARK) << LEFT << END_OUT
              << tColor(ORCHID) << filledBar << END_OUT
              << tColor(ORCHID_DARK) << emptyBar << END_OUT
              << tColor(ORCHID_DARK) << RIGHT << END_OUT
              << " " << int(ratio * 100) << "% (" << loadBar.done << "/" << loadBar.total << ")" << std::flush;
}

void Console::endLoadBar()
{
    // std::cout << "\r\033[2K" << std::flush;
    clearCursor();
    // logLn();
    // std::cout << "\n";
    loadBar.hasSeperator = false;
    showCursor();
}
#pragma endregion

#pragma region Console Output

// Outputs to console without newline
void Console::log(const string& str, const LogOptions options)
{
    // Only apply color if at least one value is present
    if (options.fg || options.bg) {
        string colorPrefix = getLogPrefix({ options.fg, options.bg });
        std::cout << colorPrefix << str << END_OUT;
    } else {
        std::cout << str; // No color applied
    }
}

// Outputs to the console and adds newline
void Console::logLn(const string& str, LogOptions options)
{
    if (options.fg || options.bg) {
        string colorPrefix = getLogPrefix({ options.fg, options.bg });
        std::cout << colorPrefix << str << END_OUT << "\n";
    } else {
        std::cout << str << "\n"; // No color applied
    }
}

// Outputs an error to the console, defaults to RED if no color is passed
void Console::warn(const string& str, LogOptions options)
{
    if (options.fg || options.bg) {
        string colorPrefix = getLogPrefix(options);
        std::cerr << colorPrefix << str << END_OUT << "\n";
    } else {
        // Default Color
        std::cerr << tColor(COLOR_WARNING) << str << END_OUT << "\n";
    }
}

// Outputs an error to the console, defaults to RED if no color is passed
void Console::error(const string& str, LogOptions options)
{
    if (options.fg || options.bg) {
        string colorPrefix = getLogPrefix(options);
        std::cerr << colorPrefix << str << END_OUT << "\n";
    } else {
        // Default Color
        std::cerr << tColor(RED) << str << END_OUT << "\n";
    }
}

// ─────────────────────────────────────────────────────────────
// Advanced Logging - High-level or formatted output helpers
//
// Description:    Handles vector printing, labeled headers, and content blocks.
// Usage:          Use for structured or styled multi-line output.
// ─────────────────────────────────────────────────────────────

// Logs a string vector in array-like format
void Console::logVector(const std::vector<string>& vec)
{
    std::cout << "[\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << " " << vec[i];
        if (i != vec.size() - 1)
            std::cout << ",\n";
        else
            std::cout << "\n";
    }
    std::cout << "]\n";
}

// Logs a formatted section header
void Console::logHeader(const string& label, LogOptions options)
{
    // Only apply color if at least one value is present
    if (options.fg || options.bg) {
        string colorPrefix = getLogPrefix({ options.fg, options.bg });
        std::cout << colorPrefix << genString("-", 27) << label
                  << genString("-", 27) << END_OUT << "\n";
    } else {
        LogOptions options = { .fg = MAGENTA, .bold = true };
        string colorPrefix = getLogPrefix(options);
        std::cout << colorPrefix << genString("-", 27) << label
                  << genString("-", 27) << END_OUT << "\n"; // No color applied
    }
}

// Logs a "page" style block with title and main content
void Console::logPage(const string& title, const string& str, const rgb& fg,
    const rgb& bg)
{
    console.log(title + ":\n" + str, { fg, bg });
}

#pragma endregion

#pragma region Console Input
string Console::selectOption(const vector<string>& options)
{
    CursorHideGuard hideCursor;

    int selected = 0;
    bool running = true;
    bool drawnOnce = false;

    auto showMenu = [&](int selected) {
        if (drawnOnce) {
            moveCursorUp(options.size());
        } else {
            drawnOnce = true;
        }

        for (int i = 0; i < options.size(); ++i) {
            clearCursor();
            if (i == selected) {
                std::cout << "> " << options[i] << "\n";
            } else {
                std::cout << "  " << options[i] << "\n";
            }
        }
    };

    while (running) {
        showMenu(selected);

        int key = _getch();

        if (key == 224) { // Arrow key prefix
            key = _getch();
            if (key == 72) { // Up arrow
                selected = (selected - 1 + options.size()) % options.size();
            } else if (key == 80) { // Down arrow
                selected = (selected + 1) % options.size();
            }
        } else if (key == 13) { // Enter key
            running = false;
        }
    }

    clear();
    return toLowerCase(options[selected]);
}

// Prompt the user for input with an optional styled message
string Console::prompt(const string& message, LogOptions options)
{
    if (!message.empty())
        log(message + " ", options); // No newline, prompt on same line

    string input;
    std::getline(std::cin, input);
    logLn();
    return trim(input); // Optionally trim whitespace, if you have a trim function
}

// Prompt for Yes/No confirmation with optional default value
bool Console::confirm(const string& question, bool defaultYes)
{
    // Example: "Proceed with overwrite? [Y/n]: "
    string suffix = defaultYes ? " [Y/n]: " : " [y/N]: ";
    string fullPrompt = question + suffix;

    while (true) {
        log(fullPrompt, { .fg = CYAN, .bold = true });
        string response;
        std::getline(std::cin, response);
        response = toLowerCase(trim(response));
        if (response.empty()) {
            return defaultYes;
        }
        if (response == "y" || response == "yes")
            return true;
        if (response == "n" || response == "no")
            return false;

        warn("Please enter 'y' or 'n'.");
    }
}

#pragma endregion

// ─────────────────────────────────────────────────────────────
// Style Presets - Common styled logging variants
//
// Description:    Predefined styled messages using fixed color themes.
// Usage:          For quick visual differentiation of log types.
// Defaults:       Uses gray for verbose, green for success, magenta for notice.
// ─────────────────────────────────────────────────────────────

// Logs a message in gray (verbose tone)
void Console::logVerbose(const string& message)
{
    console.logLn(message, { .fg = GRAY_DARK });
}

// Logs a message in green (success tone)
void Console::logLnS(const string& message)
{
    console.logLn(message, { .fg = GREEN_DARK });
}

// Logs a message in magenta with preceding line break
void Console::logLnM(const string& message)
{
    console.logLn();
    console.logLn(message, { .fg = MAGENTA });
}

// ---- Utility ----

// TODO: Test to see if background and forground gets added
string Console::getLogPrefix(LogOptions options)
{
    string front;
    string back;

    if (options.fg) {
        front = toConsoleFG(*options.fg, options.bold);
    }
    if (options.bg) {
        back = toConsoleBG(*options.bg);
    }

    return "\033[" + front + (options.bg ? ";" + back : "") + "m";
}