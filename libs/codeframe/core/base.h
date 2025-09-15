#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

#define cconst constexpr

#define EXIT true
#define CONTINUE false

// Standard Functions

template <typename T>
inline string toString(const T& value)
{
    return std::to_string(value);
}