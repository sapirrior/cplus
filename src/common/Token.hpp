#pragma once
#include <string>

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL, IDENTIFIER, STRING, NUMBER,
    AND, ELSE, FALSE, FN, FOR, IF, NULL_TOKEN, OR,
    PRINT, RETURN, TRUE, LET, WHILE, TRY, CATCH,
    END_OF_FILE, ERROR_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    double number_value = 0;
    int line = 1;
    int col = 1;
};
