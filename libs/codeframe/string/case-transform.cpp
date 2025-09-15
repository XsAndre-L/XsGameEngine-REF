#include "case-transform.h"

StringCase detectCaseStyle(const string& str)
{
    bool hasUnderscore = str.find('_') != string::npos;
    bool hasHyphen = str.find('-') != string::npos;
    bool firstIsUpper = !str.empty() && std::isupper(str[0]);

    if (hasHyphen)
        return StringCase::KebabCase;
    if (hasUnderscore)
        return StringCase::SnakeCase;
    if (firstIsUpper)
        return StringCase::PascalCase;
    return StringCase::CamelCase;
}

string toLowerCase(const string& str)
{
    string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

string toUpperCase(const string& str)
{
    string result;
    for (char c : str) {
        result += toupper(c);
    }
    return result;
}

string toCamelCase(const string& str)
{
    string result;
    bool nextUpper = false;

    for (char ch : str) {
        if (ch == '-' || ch == '_') {
            nextUpper = true;
        } else {
            result += nextUpper ? std::toupper(ch) : ch;
            nextUpper = false;
        }
    }

    return result;
}

string toPascalCase(const string& str)
{
    string camelCase = toCamelCase(str);
    camelCase[0] = std::toupper(camelCase[0]);
    return camelCase;
}

string toSnakeCase(const string& str)
{
    string result;

    for (char ch : str) {
        if (std::isupper(ch)) {
            if (!result.empty()) {
                result += '_';
            }
            result += std::tolower(ch);
        } else {
            result += ch;
        }
    }

    return result;
}

string toKebabCase(const string& str)
{
    string result;

    for (char ch : str) {
        if (std::isupper(ch)) {
            if (!result.empty()) {
                result += '-';
            }
            result += std::tolower(ch);
        } else {
            result += ch;
        }
    }

    return result;
}