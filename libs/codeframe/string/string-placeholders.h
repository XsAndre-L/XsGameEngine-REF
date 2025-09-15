#pragma once

#include <algorithm>
#include <map>
#include <regex>
#include <vector>

#include "case-transform.h"

/**
 *  @brief Replaces dynamic placeholders in a string with values from a map
 *
 *  string templateString = "Class __NAME_PASCAL__, variable __VAR_CAMEL__,
 * file __FILE_SNAKE__.html"; std::map<string, string> replacements =
 * {
 *    {"NAME", "MyClass"},
 *    {"VAR", "myVariable"},
 *    {"FILE", "my_file"},
 *    // Add more replacements as needed
 *  };
 *
 *  replaceDynamicPlaceholders(templateString, replacements);
 */

string caseTransform(const string& input,
    const string& caseType);

void replaceDynamicPlaceholders(
    string& templateStr,
    const std::map<string, string>& replacements);