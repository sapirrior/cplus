#pragma once
#include <string>

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, BANG, COLON,

    // One or two character tokens.
    BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Python-style keywords.
    FALSE, NONE, TRUE, AND, AS, ASSERT, ASYNC, AWAIT,
    BREAK, CLASS, CONTINUE, DEF, DEL, ELIF, ELSE,
    EXCEPT, FINALLY, FOR, FROM, GLOBAL, IF, IMPORT,
    IN, IS, LAMBDA, NONLOCAL, NOT, OR, PASS, RAISE,
    RETURN, TRY, WHILE, WITH, YIELD,
    
    // Custom commands.
    PRINT, LET, 

    END_OF_FILE, ERROR_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    double number_value = 0;
    int line = 1;
    int col = 1;
};
