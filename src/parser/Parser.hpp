#pragma once
#include <vector>
#include <memory>
#include "core/Common.hpp"
#include "AST.hpp"

class Parser {
    std::vector<Token> tokens;
    int current = 0;
    
    Token peek() { return tokens[current]; }
    Token previous() { return tokens[current - 1]; }
    bool isAtEnd() { return peek().type == TokenType::END_OF_FILE; }
    Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }
    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }
    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }
    Token consume(TokenType type, const std::string& message) {
        if (check(type)) return advance();
        throw ParserError(peek().line, peek().col, message);
    }

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> logical_or();
    std::shared_ptr<Expr> logical_and();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> primary();
    
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> printStatement();
    std::shared_ptr<Stmt> letStatement();
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Stmt> forStatement();
    std::shared_ptr<Stmt> functionStatement();
    std::shared_ptr<Stmt> returnStatement();
    std::shared_ptr<Stmt> inputStatement();
    std::shared_ptr<Stmt> tryCatchStatement();
    std::shared_ptr<Stmt> expressionStatement();

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
    std::vector<std::shared_ptr<Stmt>> parse();
};
