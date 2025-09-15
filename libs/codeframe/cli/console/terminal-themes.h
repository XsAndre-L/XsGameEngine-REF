#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include <core/base.h>
#include <string/string.h>

#include <theme/color.h>

using std::optional;

string toConsoleFG(const rgb& color, bool bold = false);
string toConsoleBG(const rgb& color);

string tColor(const rgb& fg, const optional<rgb>& bg = std::nullopt);

// TEXT EFFECTS
const string NORMAL = "0";
const string BOLD = "1";
// const string DIM = "2";
const string ITALIC = "3";
const string UNDERLINE = "4";
const string BLINK = "5";
const string REVERSE = "7";
const string HIDDEN = "8";
const string FAST_BLINK = "6";
const string STRIKETHROUGH = "9";

// CONSOLE END
const string END_OUT = "\033[0m";

// CLEAR SCREEN
const string CLEAR_SCREEN = "\033[2J\033[1;1H";

// CURSOR POSITION
const string CURSOR_UP = "\033[1A";
const string CURSOR_DOWN = "\033[1B";
const string CURSOR_RIGHT = "\033[1C";
const string CURSOR_LEFT = "\033[1D";

// CURSOR POSITION
const string CURSOR_POS = "\033[<L>;<C>H";

// ERASE TEXT
const string ERASE_LINE = "\033[K";

// ERASE SCREEN
const string ERASE_SCREEN = "\033[2J";

// ERASE SCREEN
const string SCROLL_UP = "\033[S";
const string SCROLL_DOWN = "\033[T";

// HIDE CURSOR
const string HIDE_CURSOR = "\033[?25l";
const string SHOW_CURSOR = "\033[?25h";

// SAVE AND RESTORE CURSOR POSITION
const string SAVE_CURSOR = "\033[s";
const string RESTORE_CURSOR = "\033[u";

string white(const string& str);
string gray(const string& str);
string red(const string& str);
string green(const string& str, bool bold = false);
string yellow(const string& str);
string blue(const string& str);
string cyan(const string& str);

void printPalette();

// 24-bit color -> should probably use this
// Foreground: \033[38;2;R;G;Bm
// Background: \033[48;2;R;G;Bm
