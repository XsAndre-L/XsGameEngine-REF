#pragma once

#include <core/base.h>

#include "keywords.h"
#include "operators.h"

enum class TokenType {
    DATA_KEYWORD,
    DB_KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    SYMBOL,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    size_t position;
};
