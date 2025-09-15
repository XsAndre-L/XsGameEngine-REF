#include "terminal-themes.h"

string toConsoleFG(const rgb& color, bool bold)
{
    return string(bold ? "1;" : "") + "38;2;" + toString(color.r) + ";" + toString(color.g) + ";" + toString(color.b);
}

string toConsoleBG(const rgb& color)
{
    return "48;2;" + toString(color.r) + ";" + toString(color.g) + ";" + toString(color.b);
}

string tColor(const rgb& fg, const optional<rgb>& bg)
{
    return "\033[" + toConsoleFG(fg) + (bg ? ";" + toConsoleBG(*bg) : "") + "m";
}

// ---------- COLORS ------------
string white(const string& str)
{
    return string(tColor(WHITE) + str + END_OUT);
}
string gray(const string& str)
{
    return string(tColor(GRAY_DARK) + str + END_OUT);
}
string black(const string& str)
{
    return string(tColor(BLACK) + str + END_OUT);
}
string red(const string& str) { return string(tColor(RED) + str + END_OUT); }
string green(const string& str, bool bold) { return tColor(GREEN) + str; }
string blue(const string& str) { return tColor(BLUE) + str + END_OUT; }
string yellow(const string& str)
{
    return tColor(YELLOW) + str + END_OUT;
    // return YELLOW + str + NORMAL);
}
string cyan(const string& str)
{
    return tColor(CYAN) + str + END_OUT;
    // return CYAN + str + NORMAL);
}

// ---------- THEMES ------------

void printPalette()
{
    std::cout << "Palette: " << '\n';
    std::cout << "  " << white("██") << '\n';
    std::cout << "  " << black("██") << '\n';
    std::cout << "  " << red("██") << '\n';
    std::cout << "  " << green("██") << '\n';
    std::cout << "  " << yellow("██") << '\n';
    std::cout << "  " << blue("██") << '\n';
    std::cout << "  " << cyan("██") << '\n';
}
