#include "string.h"

vector<string> splitString(const string& input, char delimiter)
{
    vector<string> tokens;
    std::istringstream iss(input);
    string token;

    while (getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void ltrim(string& s)
{
    size_t i = 0;
    while (i < s.size() && std::isspace((unsigned char)s[i]))
        ++i;
    s.erase(0, i);
}
void rtrim(string& s)
{
    size_t i = s.size();
    while (i && std::isspace((unsigned char)s[i - 1]))
        --i;
    s.erase(i);
}
string trim(string s)
{
    ltrim(s);
    rtrim(s);
    return s;
}

// string trim(const string& str)
// {
//     auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
//     auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
//     return (start < end) ? string(start, end) : "";
// }

// tri

string replaceAll(string& str, const string& from, const string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

// Replace all keys in map with their values in a string
string replaceAllInMap(const string& input, const StringReplaceMap& replaceMap)
{
    string out = input;
    for (const auto& [key, value] : replaceMap) {
        size_t pos = 0;
        while ((pos = out.find(key, pos)) != string::npos) {
            out.replace(pos, key.length(), value);
            pos += value.length();
        }
    }
    return out;
}

string joinStringsWithNewline(const vector<string>& strings)
{
    string joinedStrings;
    size_t totalSize = 0;

    for (const auto& str : strings) {
        totalSize += str.size() + 1; // +1 for newline
    }

    joinedStrings.reserve(totalSize);

    for (const auto& str : strings) {
        joinedStrings += str;
        joinedStrings += '\n';
    }

    return joinedStrings;
}

string genString(const string& character, int length)
{
    return string(length, character[0]);
}

// ----- VECTOR UTIL FUNCTIONS ------

void removeIndexes(vector<string>& vec, vector<int> indexes)
{
    // for (int i = 0; i < indexes.size(); i++)
    // {
    //   vec.erase(vec.begin() + indexes[i]);
    // }

    // Remove from back to front
    for (int i = indexes.size() - 1; i >= 0; --i) {
        vec.erase(vec.begin() + indexes[i]);
    }
}

void skipWhitespace(const string& input, size_t& pos)
{
    while (pos < input.length() && std::isspace(static_cast<unsigned char>(input[pos]))) {
        pos++;
    }
}