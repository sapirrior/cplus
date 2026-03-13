#pragma once
#include <string>
#include <vector>
#include "core/Common.hpp"

class Lexer {
    std::string source;
    int start = 0;
    int current = 0;
    int line = 1;
    int col = 1;
    int start_col = 1;
    
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
    bool isAtEnd();
    Token makeToken(TokenType type);
public:
    Lexer(const std::string& source) : source(source) {}
    std::vector<Token> scanTokens();
};
