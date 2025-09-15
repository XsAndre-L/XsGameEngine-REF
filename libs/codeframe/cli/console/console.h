#pragma once

#include <conio.h> // For _getch() on Windows
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "terminal-themes.h"

using std::string, std::optional, std::vector;

struct LogOptions {
    std::optional<rgb> fg = std::nullopt;
    std::optional<rgb> bg = std::nullopt;
    bool bold = false;
    bool underline = false;
    // …add anything else you like…
};

struct LoadBarState {
    std::size_t done = 0;
    std::size_t total = 0;
    std::string label;
    bool hasSeperator = false;
    bool active = false;
};

// ─────────────────────────────────────────────────────────────
// Console - Unified interface for terminal output styling
//
// Description:    Handles formatted console output with RGB color support.
// Usage:          Use `console.log(...)` and related helpers for styled output.
// ─────────────────────────────────────────────────────────────
class Console {
public:
    struct CursorHideGuard {
        CursorHideGuard() { std::cout << "\x1b[?25l"; }
        ~CursorHideGuard() { std::cout << "\x1b[?25h"; }
    };

    void hideCursor();
    void showCursor();
    void moveCursorUp(const size_t& lines);
    void clearCursor();

    void clear();

    LoadBarState loadBar;
    void startLoadBar(std::size_t total, const string& label, const string& log = " ");
    void updateLoadBar(std::size_t delta, const string& log = "");
    void endLoadBar();

    // ---- Basic Logging ----
    // void log(const string &str = "", const optional<rgb> &fg = std::nullopt,
    //          const optional<rgb> &bg = std::nullopt);
    void log(const string& str = "", LogOptions options = {});
    void logLn(const string& str = "", LogOptions options = {});
    void warn(const string& str = "", LogOptions options = {});
    void error(const string& str = "", LogOptions options = {});

    // ---- Advanced Logging ----
    void logHeader(const string& label, LogOptions options = {});
    void logPage(const string& title, const string& str, const rgb& fg = GRAY,
        const rgb& bg = rgb(10, 10, 10));
    void logVector(const vector<string>& vec);

    template <typename Container>
    void logContainer(const Container& container)
    {
        std::cout << "[\n";
        for (auto it = container.begin(); it != container.end();) {
            std::cout << " " << *it;
            if (++it != container.end())
                std::cout << ",\n";
            else
                std::cout << "\n";
        }
        std::cout << "]\n";
    }

    string selectOption(const vector<string>& options);

    // ---- Input Handling ----
    string prompt(const string& message, LogOptions options = {});
    // string promptSecret(const string& message);
    bool confirm(const string& question, bool defaultYes = true);

    // ---- Style Preset ----
    void logVerbose(const string& message);
    void logLnS(const string& message);
    void logLnM(const string& message);

private:
    // ---- Utility ----
    string getLogPrefix(LogOptions options);
    // string getLogPrefix(const optional<rgb> &fg = std::nullopt,
    //                         const optional<rgb> &bg = std::nullopt);
};

extern Console console;
