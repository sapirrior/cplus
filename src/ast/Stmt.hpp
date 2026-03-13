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
public:
    std::vector<std::shared_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::shared_ptr<Stmt>> s) : statements(s) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ExpressionStmt : public Stmt {
public:
    std::shared_ptr<Expr> expression;
    ExpressionStmt(std::shared_ptr<Expr> e) : expression(e) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class LetStmt : public Stmt {
public:
    Token name; std::shared_ptr<Expr> initializer;
    LetStmt(Token n, std::shared_ptr<Expr> i) : name(n), initializer(i) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class IfStmt : public Stmt {
public:
    struct ElifBranch { std::shared_ptr<Expr> condition; std::shared_ptr<Stmt> body; };
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::vector<ElifBranch> elifBranches;
    std::shared_ptr<Stmt> elseBranch;
    IfStmt(std::shared_ptr<Expr> c, std::shared_ptr<Stmt> t, std::vector<ElifBranch> eb, std::shared_ptr<Stmt> e) 
        : condition(c), thenBranch(t), elifBranches(eb), elseBranch(e) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class WhileStmt : public Stmt {
public:
    std::shared_ptr<Expr> condition; std::shared_ptr<Stmt> body;
    WhileStmt(std::shared_ptr<Expr> c, std::shared_ptr<Stmt> b) : condition(c), body(b) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class FunctionStmt : public Stmt {
public:
    Token name; std::vector<Token> params; std::shared_ptr<Stmt> body;
    FunctionStmt(Token n, std::vector<Token> p, std::shared_ptr<Stmt> b) : name(n), params(p), body(b) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ReturnStmt : public Stmt {
public:
    Token keyword; std::shared_ptr<Expr> value;
    ReturnStmt(Token k, std::shared_ptr<Expr> v) : keyword(k), value(v) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class PrintStmt : public Stmt {
public:
    std::shared_ptr<Expr> expression;
    PrintStmt(std::shared_ptr<Expr> e) : expression(e) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class TryCatchStmt : public Stmt {
public:
    std::shared_ptr<Stmt> tryBlock; std::shared_ptr<Stmt> catchBlock;
    TryCatchStmt(std::shared_ptr<Stmt> t, std::shared_ptr<Stmt> c) : tryBlock(t), catchBlock(c) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class BreakStmt : public Stmt { public: void execute(std::shared_ptr<Environment> env) override; };
class ContinueStmt : public Stmt { public: void execute(std::shared_ptr<Environment> env) override; };
class PassStmt : public Stmt { public: void execute(std::shared_ptr<Environment> env) override; };

struct BreakException {};
struct ContinueException {};
