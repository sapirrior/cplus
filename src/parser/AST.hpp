#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/Common.hpp"
#include "core/Environment.hpp"

class Expr {
public:
    virtual ~Expr() = default;
    virtual Value evaluate(std::shared_ptr<Environment> env) = 0;
};

class BinaryExpr : public Expr {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
public:
    BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class LogicalExpr : public Expr {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
public:
    LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(left), op(op), right(right) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class UnaryExpr : public Expr {
    Token op;
    std::shared_ptr<Expr> right;
public:
    UnaryExpr(Token op, std::shared_ptr<Expr> right) : op(op), right(right) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class LiteralExpr : public Expr {
    Value value;
public:
    LiteralExpr(Value value) : value(value) {}
    Value evaluate(std::shared_ptr<Environment> env) override { return value; }
};

class GroupingExpr : public Expr {
    std::shared_ptr<Expr> expression;
public:
    GroupingExpr(std::shared_ptr<Expr> expression) : expression(expression) {}
    Value evaluate(std::shared_ptr<Environment> env) override { return expression->evaluate(env); }
};

class VariableExpr : public Expr {
    Token name;
public:
    VariableExpr(Token name) : name(name) {}
    Token getToken() const { return name; }
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class ArrayAccessExpr : public Expr {
    std::shared_ptr<Expr> array;
    std::shared_ptr<Expr> index;
public:
    ArrayAccessExpr(std::shared_ptr<Expr> array, std::shared_ptr<Expr> index) : array(array), index(index) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class CallExpr : public Expr {
    Token callee;
    std::vector<std::shared_ptr<Expr>> arguments;
public:
    CallExpr(Token callee, std::vector<std::shared_ptr<Expr>> arguments) : callee(callee), arguments(arguments) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void execute(std::shared_ptr<Environment> env) = 0;
};

class PrintStmt : public Stmt {
    std::shared_ptr<Expr> expression;
public:
    PrintStmt(std::shared_ptr<Expr> expression) : expression(expression) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class LetStmt : public Stmt {
    Token name;
    std::shared_ptr<Expr> initializer;
public:
    LetStmt(Token name, std::shared_ptr<Expr> initializer) : name(name), initializer(initializer) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class IfStmt : public Stmt {
    std::shared_ptr<Expr> condition;
    std::vector<std::shared_ptr<Stmt>> thenBranch;
    std::vector<std::shared_ptr<Stmt>> elseBranch;
public:
    IfStmt(std::shared_ptr<Expr> condition, std::vector<std::shared_ptr<Stmt>> thenBranch, std::vector<std::shared_ptr<Stmt>> elseBranch) 
        : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ForStmt : public Stmt {
    Token name;
    std::shared_ptr<Expr> start;
    std::shared_ptr<Expr> end;
    std::vector<std::shared_ptr<Stmt>> body;
public:
    ForStmt(Token name, std::shared_ptr<Expr> start, std::shared_ptr<Expr> end, std::vector<std::shared_ptr<Stmt>> body) : name(name), start(start), end(end), body(body) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class FunctionStmt : public Stmt {
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
public:
    FunctionStmt(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body) : name(name), params(params), body(body) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ReturnStmt : public Stmt {
    Token keyword;
    std::shared_ptr<Expr> value;
public:
    ReturnStmt(Token keyword, std::shared_ptr<Expr> value) : keyword(keyword), value(value) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class InputStmt : public Stmt {
    Token name;
public:
    InputStmt(Token name) : name(name) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class TryCatchStmt : public Stmt {
    std::vector<std::shared_ptr<Stmt>> tryBranch;
    std::vector<std::shared_ptr<Stmt>> catchBranch;
public:
    TryCatchStmt(std::vector<std::shared_ptr<Stmt>> tryBranch, std::vector<std::shared_ptr<Stmt>> catchBranch) 
        : tryBranch(tryBranch), catchBranch(catchBranch) {}
    void execute(std::shared_ptr<Environment> env) override;
};

class ExpressionStmt : public Stmt {
    std::shared_ptr<Expr> expression;
public:
    ExpressionStmt(std::shared_ptr<Expr> expression) : expression(expression) {}
    void execute(std::shared_ptr<Environment> env) override;
};

struct ReturnException { Value value; };
