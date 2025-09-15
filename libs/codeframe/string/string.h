#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "case-transform.h"
#include "string-placeholders.h"

using std::string, std::vector;

// ... or more semantic:
using StringReplaceMap = std::unordered_map<string, string>;

// STRING FUNCTIONS ----------------------------

vector<string> splitString(const string& input, char delimiter);

// Advances the given position past all whitespace characters (space, tab, newline, etc.)
// Parameters:
// - input: the full input string to scan
// - pos: a reference to the current position in the string, which will be advanced
//        until the first non-whitespace character or end of string is reached
void skipWhitespace(const string& input, size_t& pos);

void ltrim(string& s);
void rtrim(string& s);
string trim(string s);

string replaceAll(string& str, const string& from, const string& to);

string replaceAllInMap(const std::string& input, const StringReplaceMap& replaceMap);

string joinStringsWithNewline(const vector<string>& strings);

string genString(const string& character, int length);

// VECTOR FUNCTIONS ----------------------------

void removeIndexes(vector<string>& vec, vector<int> indexes);