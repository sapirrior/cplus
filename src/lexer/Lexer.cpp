#include "Lexer.hpp"
#include <cctype>
#include <unordered_map>

static std::unordered_map<std::string, TokenType> keywords = {
    {"AND", TokenType::AND}, {"OR", TokenType::OR}, {"NOT", TokenType::NOT},
    {"PRINT", TokenType::PRINT}, {"LET", TokenType::LET}, {"IF", TokenType::IF},
    {"THEN", TokenType::THEN}, {"ELSE", TokenType::ELSE}, {"END", TokenType::END},
    {"FOR", TokenType::FOR}, {"TO", TokenType::TO}, {"NEXT", TokenType::NEXT},
    {"FUNCTION", TokenType::FUNCTION}, {"CALL", TokenType::CALL}, {"RETURN", TokenType::RETURN},
    {"INPUT", TokenType::INPUT}, {"REM", TokenType::REM}, {"TRY", TokenType::TRY},
    {"CATCH", TokenType::CATCH}, {"ARRAY", TokenType::ARRAY}
};

char Lexer::advance() {
    current++;
    col++;
    return source[current - 1];
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    col++;
    return true;
}

bool Lexer::isAtEnd() {
    return current >= (int)source.length();
}

Token Lexer::makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.lexeme = source.substr(start, current - start);
    token.line = line;
    token.col = start_col;
    return token;
}

std::vector<Token> Lexer::scanTokens() {
    std::vector<Token> tokens;
    while (!isAtEnd()) {
        start = current;
        start_col = col;
        char c = advance();
        
        switch (c) {
            case '(': tokens.push_back(makeToken(TokenType::LEFT_PAREN)); break;
            case ')': tokens.push_back(makeToken(TokenType::RIGHT_PAREN)); break;
            case '[': tokens.push_back(makeToken(TokenType::LEFT_BRACKET)); break;
            case ']': tokens.push_back(makeToken(TokenType::RIGHT_BRACKET)); break;
            case ',': tokens.push_back(makeToken(TokenType::COMMA)); break;
            case '.': tokens.push_back(makeToken(TokenType::DOT)); break;
            case '-': tokens.push_back(makeToken(TokenType::MINUS)); break;
            case '+': tokens.push_back(makeToken(TokenType::PLUS)); break;
            case '*': tokens.push_back(makeToken(TokenType::STAR)); break;
            case '/': tokens.push_back(makeToken(TokenType::SLASH)); break;
            case '#': // Comment
                while (peek() != '\n' && !isAtEnd()) advance();
                break;
            case '=':
                tokens.push_back(makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL));
                break;
            case '<':
                if (match('>')) tokens.push_back(makeToken(TokenType::BANG_EQUAL));
                else if (match('=')) tokens.push_back(makeToken(TokenType::LESS_EQUAL));
                else tokens.push_back(makeToken(TokenType::LESS));
                break;
            case '>':
                tokens.push_back(makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER));
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                tokens.push_back(makeToken(TokenType::NEWLINE));
                line++;
                col = 1;
                break;
            case '"': {
                while (peek() != '"' && !isAtEnd()) {
                    if (peek() == '\n') { line++; col = 1; }
                    advance();
                }
                if (isAtEnd()) {
                    tokens.push_back(makeToken(TokenType::ERROR_TOKEN)); 
                    break;
                }
                advance(); 
                Token t = makeToken(TokenType::STRING);
                t.lexeme = source.substr(start + 1, current - start - 2);
                tokens.push_back(t);
                break;
            }
            default:
                if (std::isdigit(c)) {
                    while (std::isdigit(peek())) advance();
                    if (peek() == '.' && std::isdigit(peekNext())) {
                        advance(); 
                        while (std::isdigit(peek())) advance();
                    }
                    Token t = makeToken(TokenType::NUMBER);
                    t.number_value = std::stod(t.lexeme);
                    tokens.push_back(t);
                } else if (std::isalpha(c)) {
                    while (std::isalnum(peek()) || peek() == '_') advance();
                    std::string text = source.substr(start, current - start);
                    
                    if (text == "REM") {
                        while (peek() != '\n' && !isAtEnd()) advance();
                        break;
                    }
                    
                    if (keywords.count(text)) {
                        tokens.push_back(makeToken(keywords[text]));
                    } else {
                        tokens.push_back(makeToken(TokenType::IDENTIFIER));
                    }
                } else {
                    tokens.push_back(makeToken(TokenType::ERROR_TOKEN));
                }
                break;
        }
    }
    
    start = current;
    start_col = col;
    tokens.push_back(makeToken(TokenType::END_OF_FILE));
    return tokens;
}
