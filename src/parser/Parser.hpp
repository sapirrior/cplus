#pragma once
#include <vector>
#include <memory>
#include "../common/Token.hpp"
#include "../error/Exceptions.hpp"
#include "../ast/Expr.hpp"
#include "../ast/Stmt.hpp"

class Parser {
    std::vector<Token> tokens;
    int current = 0;
    
    Token peek();
    Token previous();
    bool isAtEnd();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
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
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> functionDeclaration();
    std::shared_ptr<Stmt> letDeclaration();
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Stmt> whileStatement();
    std::shared_ptr<Stmt> forStatement();
    std::shared_ptr<Stmt> returnStatement();
    std::shared_ptr<Stmt> tryCatchStatement();
    std::shared_ptr<Stmt> printStatement();
    std::shared_ptr<Stmt> block();
    std::shared_ptr<Stmt> expressionStatement();

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
    std::vector<std::shared_ptr<Stmt>> parse();
};
