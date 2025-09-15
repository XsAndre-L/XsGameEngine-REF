#pragma once

#include <core/base.h>
#include <string/string.h>
#include <unordered_map>

#include "tokens.h"

// The Tokenizer breaks a raw SQL string into atomic chunks — called tokens — that the parser can understand.

class Tokenizer {
public:
    Tokenizer(const string& input)
        : input(input)
        , pos(0)
    {
    }

    vector<Token> tokenize();

private:
    string input;
    size_t pos;
    std::unordered_map<string, TokenType> keywords = {
        { "SELECT", TokenType::DATA_KEYWORD }, { "FROM", TokenType::DATA_KEYWORD },
        { "WHERE", TokenType::DATA_KEYWORD }, { "INSERT", TokenType::DATA_KEYWORD },
        { "INTO", TokenType::DATA_KEYWORD }, { "VALUES", TokenType::DATA_KEYWORD },
        { "UPDATE", TokenType::DATA_KEYWORD }, { "DELETE", TokenType::DATA_KEYWORD }
    };

    char peek() const { return input[pos]; }
    char advance() { return input[pos++]; }

    Token readWord();

    Token readNumber();

    Token readString();

    Token readOperator();

    bool isOperator(char c) const { return c == '=' || c == '<' || c == '>' || c == '!'; }
    bool isSymbol(char c) const { return c == ',' || c == ';' || c == '(' || c == ')' || c == '*'; }

    string toUpper(const string& str)
    {
        string result = str;
        for (char& c : result)
            c = std::toupper(c);
        return result;
    }
};