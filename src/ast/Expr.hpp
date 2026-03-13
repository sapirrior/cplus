#pragma once
#include <memory>
#include <vector>
#include "../common/Token.hpp"
#include "../common/Value.hpp"

class Environment;

class Expr {
public:
    virtual ~Expr() = default;
    virtual Value evaluate(std::shared_ptr<Environment> env) = 0;
};

class BinaryExpr : public Expr {
public:
    std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right;
    BinaryExpr(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r) : left(l), op(o), right(r) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class LogicalExpr : public Expr {
public:
    std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right;
    LogicalExpr(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r) : left(l), op(o), right(r) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class UnaryExpr : public Expr {
public:
    Token op; std::shared_ptr<Expr> right;
    UnaryExpr(Token o, std::shared_ptr<Expr> r) : op(o), right(r) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class LiteralExpr : public Expr {
public:
    Value value;
    LiteralExpr(Value v) : value(v) {}
    Value evaluate(std::shared_ptr<Environment> env) override { return value; }
};

class VariableExpr : public Expr {
public:
    Token name;
    VariableExpr(Token n) : name(n) {}
    Token getToken() const { return name; }
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class AssignExpr : public Expr {
public:
    Token name; std::shared_ptr<Expr> value;
    AssignExpr(Token n, std::shared_ptr<Expr> v) : name(n), value(v) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class ArrayAssignExpr : public Expr {
public:
    std::shared_ptr<Expr> array; std::shared_ptr<Expr> index; std::shared_ptr<Expr> value;
    ArrayAssignExpr(std::shared_ptr<Expr> a, std::shared_ptr<Expr> i, std::shared_ptr<Expr> v) : array(a), index(i), value(v) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class ArrayAccessExpr : public Expr {
public:
    std::shared_ptr<Expr> array; std::shared_ptr<Expr> index;
    ArrayAccessExpr(std::shared_ptr<Expr> a, std::shared_ptr<Expr> i) : array(a), index(i) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class CallExpr : public Expr {
public:
    std::shared_ptr<Expr> callee; Token paren; std::vector<std::shared_ptr<Expr>> arguments;
    CallExpr(std::shared_ptr<Expr> c, Token p, std::vector<std::shared_ptr<Expr>> a) : callee(c), paren(p), arguments(a) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};

class ArrayLiteralExpr : public Expr {
public:
    std::vector<std::shared_ptr<Expr>> elements;
    ArrayLiteralExpr(std::vector<std::shared_ptr<Expr>> e) : elements(e) {}
    Value evaluate(std::shared_ptr<Environment> env) override;
};
