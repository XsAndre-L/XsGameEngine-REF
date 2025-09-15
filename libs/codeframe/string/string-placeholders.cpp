
#include "string-placeholders.h"

string caseTransform(const string& input,
    const string& caseType)
{
    if (caseType == "PASCAL") {
        return toPascalCase(input);
    } else if (caseType == "CAMEL") {
        return toCamelCase(input);
    } else if (caseType == "SNAKE") {
        return toSnakeCase(input);
    } else if (caseType == "KEBAB") {
        return toKebabCase(input);
    }
    return input; // default, no transformation
}

void replaceDynamicPlaceholders(
    string& templateStr,
    const std::map<string, string>& replacements)
{
    std::regex placeholderRegex(
        "\\{\\{([A-Z]+)_(PASCAL|CAMEL|SNAKE|KEBAB)\\}\\}");

    std::smatch match;
    string::const_iterator searchStart(templateStr.cbegin());
    while (std::regex_search(searchStart, templateStr.cend(), match,
        placeholderRegex)) {
        string fullMatch = match.str(0);
        string varName = match.str(1);
        string caseType = match.str(2);

        // Transform and replace if replacement is found
        auto replacementIt = replacements.find(varName);
        if (replacementIt != replacements.end()) {
            string replacement = caseTransform(replacementIt->second, caseType);

            templateStr.replace(templateStr.find(fullMatch), fullMatch.length(),
                replacement);
            // Calculate the new starting position.
            searchStart = templateStr.begin() + match.position(0) + replacement.length();
        } else {
            // If no replacement was made, increment searchStart normally.
            searchStart = match.suffix().first;
        }
    }
}