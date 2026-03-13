#include "Lexer.hpp"
#include <cctype>
#include <unordered_map>

static std::unordered_map<std::string, TokenType> keywords = {
    {"False", TokenType::FALSE}, {"None", TokenType::NONE}, {"True", TokenType::TRUE},
    {"and", TokenType::AND}, {"as", TokenType::AS}, {"assert", TokenType::ASSERT},
    {"async", TokenType::ASYNC}, {"await", TokenType::AWAIT}, {"break", TokenType::BREAK},
    {"class", TokenType::CLASS}, {"continue", TokenType::CONTINUE}, {"def", TokenType::DEF},
    {"del", TokenType::DEL}, {"elif", TokenType::ELIF}, {"else", TokenType::ELSE},
    {"except", TokenType::EXCEPT}, {"finally", TokenType::FINALLY}, {"for", TokenType::FOR},
    {"from", TokenType::FROM}, {"global", TokenType::GLOBAL}, {"if", TokenType::IF},
    {"import", TokenType::IMPORT}, {"in", TokenType::IN}, {"is", TokenType::IS},
    {"lambda", TokenType::LAMBDA}, {"nonlocal", TokenType::NONLOCAL}, {"not", TokenType::NOT},
    {"or", TokenType::OR}, {"pass", TokenType::PASS}, {"raise", TokenType::RAISE},
    {"return", TokenType::RETURN}, {"try", TokenType::TRY}, {"while", TokenType::WHILE},
    {"with", TokenType::WITH}, {"yield", TokenType::YIELD},
    // Keep helper commands for now.
    {"print", TokenType::PRINT}, {"let", TokenType::LET}
};

char Lexer::advance() { current++; col++; return source[current - 1]; }
char Lexer::peek() { if (isAtEnd()) return '\0'; return source[current]; }
char Lexer::peekNext() { if (current + 1 >= (int)source.length()) return '\0'; return source[current + 1]; }
bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++; col++; return true;
}
bool Lexer::isAtEnd() { return current >= (int)source.length(); }
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
        start = current; start_col = col;
        char c = advance();
        switch (c) {
            case '(': tokens.push_back(makeToken(TokenType::LEFT_PAREN)); break;
            case ')': tokens.push_back(makeToken(TokenType::RIGHT_PAREN)); break;
            case '{': tokens.push_back(makeToken(TokenType::LEFT_BRACE)); break;
            case '}': tokens.push_back(makeToken(TokenType::RIGHT_BRACE)); break;
            case '[': tokens.push_back(makeToken(TokenType::LEFT_BRACKET)); break;
            case ']': tokens.push_back(makeToken(TokenType::RIGHT_BRACKET)); break;
            case ',': tokens.push_back(makeToken(TokenType::COMMA)); break;
            case '.': tokens.push_back(makeToken(TokenType::DOT)); break;
            case '-': tokens.push_back(makeToken(TokenType::MINUS)); break;
            case '+': tokens.push_back(makeToken(TokenType::PLUS)); break;
            case ';': tokens.push_back(makeToken(TokenType::SEMICOLON)); break;
            case ':': tokens.push_back(makeToken(TokenType::COLON)); break;
            case '*': tokens.push_back(makeToken(TokenType::STAR)); break;
            case '!': tokens.push_back(makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG)); break;
            case '=': tokens.push_back(makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL)); break;
            case '<': tokens.push_back(makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS)); break;
            case '>': tokens.push_back(makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER)); break;
            case '/':
                if (match('/')) { while (peek() != '\n' && !isAtEnd()) advance(); }
                else { tokens.push_back(makeToken(TokenType::SLASH)); }
                break;
            case ' ': case '\r': case '\t': break;
            case '\n': line++; col = 1; break;
            case '"': {
                while (peek() != '"' && !isAtEnd()) { if (peek() == '\n') { line++; col = 1; } advance(); }
                if (isAtEnd()) { tokens.push_back(makeToken(TokenType::ERROR_TOKEN)); break; }
                advance(); Token t = makeToken(TokenType::STRING);
                t.lexeme = source.substr(start + 1, current - start - 2);
                tokens.push_back(t); break;
            }
            default:
                if (std::isdigit(c)) {
                    while (std::isdigit(peek())) advance();
                    if (peek() == '.' && std::isdigit(peekNext())) { advance(); while (std::isdigit(peek())) advance(); }
                    Token t = makeToken(TokenType::NUMBER);
                    t.number_value = std::stod(t.lexeme);
                    tokens.push_back(t);
                } else if (std::isalpha(c) || c == '_') {
                    while (std::isalnum(peek()) || peek() == '_') advance();
                    std::string text = source.substr(start, current - start);
                    if (keywords.count(text)) tokens.push_back(makeToken(keywords[text]));
                    else tokens.push_back(makeToken(TokenType::IDENTIFIER));
                } else tokens.push_back(makeToken(TokenType::ERROR_TOKEN));
                break;
        }
    }
    start = current; start_col = col;
    tokens.push_back(makeToken(TokenType::END_OF_FILE));
    return tokens;
}
