#pragma once

#include "parser.h"
#include "tokenizer/tokenizer.h"

std::string readSQL();

std::vector<Token> tokenizeSQL(const std::string& input);
std::unique_ptr<SQLStatement> parseSQL(const std::vector<Token>& tokens);
ResultSet executeSQL(const SQLStatement& stmt);

ResultSet runSQL(const std::string& sql);