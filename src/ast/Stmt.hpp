#pragma once
#include <memory>
#include <vector>
#include "Expr.hpp"

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void execute(std::shared_ptr<Environment> env) = 0;
};

class BlockStmt : public Stmt {
    std::vector<std::shared_ptr<Stmt>> statements;
public:
    BlockStmt(std::vector<std::shared_ptr<Stmt>> s) : statements(s) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ExpressionStmt : public Stmt {
    std::shared_ptr<Expr> expression;
public:
    ExpressionStmt(std::shared_ptr<Expr> e) : expression(e) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class LetStmt : public Stmt {
    Token name; std::shared_ptr<Expr> initializer;
public:
    LetStmt(Token n, std::shared_ptr<Expr> i) : name(n), initializer(i) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class IfStmt : public Stmt {
    std::shared_ptr<Expr> condition; std::shared_ptr<Stmt> thenBranch; std::shared_ptr<Stmt> elseBranch;
public:
    IfStmt(std::shared_ptr<Expr> c, std::shared_ptr<Stmt> t, std::shared_ptr<Stmt> e) : condition(c), thenBranch(t), elseBranch(e) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class WhileStmt : public Stmt {
    std::shared_ptr<Expr> condition; std::shared_ptr<Stmt> body;
public:
    WhileStmt(std::shared_ptr<Expr> c, std::shared_ptr<Stmt> b) : condition(c), body(b) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class FunctionStmt : public Stmt {
    Token name; std::vector<Token> params; std::shared_ptr<Stmt> body;
public:
    FunctionStmt(Token n, std::vector<Token> p, std::shared_ptr<Stmt> b) : name(n), params(p), body(b) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ReturnStmt : public Stmt {
    Token keyword; std::shared_ptr<Expr> value;
public:
    ReturnStmt(Token k, std::shared_ptr<Expr> v) : keyword(k), value(v) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class PrintStmt : public Stmt {
    std::shared_ptr<Expr> expression;
public:
    PrintStmt(std::shared_ptr<Expr> e) : expression(e) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class TryCatchStmt : public Stmt {
    std::shared_ptr<Stmt> tryBlock; std::shared_ptr<Stmt> catchBlock;
public:
    TryCatchStmt(std::shared_ptr<Stmt> t, std::shared_ptr<Stmt> c) : tryBlock(t), catchBlock(c) {}
    void execute(std::shared_ptr<Environment> env) override;
};
