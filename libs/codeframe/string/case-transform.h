#pragma once

#include <iostream>

#include "../core/base.h"

enum class StringCase { Unknown,
    CamelCase,
    PascalCase,
    SnakeCase,
    KebabCase };

StringCase detectCaseStyle(const string& str);

string toLowerCase(const string& str);

string toUpperCase(const string& str);

string toCamelCase(const string& str);

string toPascalCase(const string& str);

string toSnakeCase(const string& str);

string toKebabCase(const string& str);