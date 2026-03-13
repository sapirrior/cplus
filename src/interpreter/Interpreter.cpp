#include "parser/AST.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

// Built-in functions implementation
Value callBuiltIn(const std::string& name, const std::vector<Value>& args, int line, int col) {
    if (name == "SQRT") {
        if (args.size() != 1) throw RuntimeError(line, col, "SQRT expects 1 argument.");
        return Value(std::sqrt(args[0].asNumber()));
    }
    if (name == "SIN") {
        if (args.size() != 1) throw RuntimeError(line, col, "SIN expects 1 argument.");
        return Value(std::sin(args[0].asNumber()));
    }
    if (name == "COS") {
        if (args.size() != 1) throw RuntimeError(line, col, "COS expects 1 argument.");
        return Value(std::cos(args[0].asNumber()));
    }
    if (name == "ROUND") {
        if (args.size() != 1) throw RuntimeError(line, col, "ROUND expects 1 argument.");
        return Value(std::round(args[0].asNumber()));
    }
    if (name == "RAND") {
        return Value((double)rand() / RAND_MAX);
    }
    if (name == "LEN") {
        if (args.size() != 1) throw RuntimeError(line, col, "LEN expects 1 argument.");
        if (args[0].isString()) return Value((double)args[0].asString().length());
        if (args[0].isArray()) return Value((double)args[0].asArray().size());
        throw RuntimeError(line, col, "LEN expects string or array.");
    }
    throw RuntimeError(line, col, "Undefined function '" + name + "'.");
}

Value BinaryExpr::evaluate(std::shared_ptr<Environment> env) {
    Value l = left->evaluate(env);
    Value r = right->evaluate(env);
    
    switch (op.type) {
        case TokenType::PLUS:
            if (l.isNumber() && r.isNumber()) return Value(l.asNumber() + r.asNumber());
            if (l.isString() && r.isString()) return Value(l.asString() + r.asString());
            throw RuntimeError(op.line, op.col, "Operands must be two numbers or two strings.");
        case TokenType::MINUS:
            return Value(l.asNumber() - r.asNumber());
        case TokenType::STAR:
            return Value(l.asNumber() * r.asNumber());
        case TokenType::SLASH:
            if (r.asNumber() == 0) throw RuntimeError(op.line, op.col, "Division by zero.");
            return Value(l.asNumber() / r.asNumber());
        case TokenType::GREATER:
            return Value(l.asNumber() > r.asNumber() ? 1.0 : 0.0);
        case TokenType::LESS:
            return Value(l.asNumber() < r.asNumber() ? 1.0 : 0.0);
        case TokenType::GREATER_EQUAL:
            return Value(l.asNumber() >= r.asNumber() ? 1.0 : 0.0);
        case TokenType::LESS_EQUAL:
            return Value(l.asNumber() <= r.asNumber() ? 1.0 : 0.0);
        case TokenType::EQUAL_EQUAL:
            if (l.isNumber() && r.isNumber()) return Value(l.asNumber() == r.asNumber() ? 1.0 : 0.0);
            if (l.isString() && r.isString()) return Value(l.asString() == r.asString() ? 1.0 : 0.0);
            return Value(0.0);
        case TokenType::BANG_EQUAL:
            if (l.isNumber() && r.isNumber()) return Value(l.asNumber() != r.asNumber() ? 1.0 : 0.0);
            if (l.isString() && r.isString()) return Value(l.asString() != r.asString() ? 1.0 : 0.0);
            return Value(1.0);
        default: break;
    }
    throw RuntimeError(op.line, op.col, "Unknown binary operator.");
}

Value LogicalExpr::evaluate(std::shared_ptr<Environment> env) {
    Value l = left->evaluate(env);
    if (op.type == TokenType::OR) {
        if (l.isTruthy()) return l;
    } else {
        if (!l.isTruthy()) return l;
    }
    return right->evaluate(env);
}

Value UnaryExpr::evaluate(std::shared_ptr<Environment> env) {
    Value r = right->evaluate(env);
    if (op.type == TokenType::NOT) return Value(r.isTruthy() ? 0.0 : 1.0);
    if (op.type == TokenType::MINUS) return Value(-r.asNumber());
    return Value();
}

Value VariableExpr::evaluate(std::shared_ptr<Environment> env) {
    return env->get(name.lexeme);
}

Value ArrayAccessExpr::evaluate(std::shared_ptr<Environment> env) {
    Value arrVal = array->evaluate(env);
    Value idxVal = index->evaluate(env);
    if (!arrVal.isArray()) throw RuntimeError(0, 0, "Target is not an array.");
    if (!idxVal.isNumber()) throw RuntimeError(0, 0, "Index is not a number.");
    auto arr = arrVal.asArray();
    size_t i = (size_t)idxVal.asNumber();
    if (i >= arr.size()) throw RuntimeError(0, 0, "Array index out of bounds.");
    return *arr[i];
}

Value CallExpr::evaluate(std::shared_ptr<Environment> env) {
    std::vector<Value> args;
    for (auto& arg : arguments) args.push_back(arg->evaluate(env));
    
    auto func = env->getFunction(callee.lexeme);
    if (func == nullptr) return callBuiltIn(callee.lexeme, args, callee.line, callee.col);
    
    if (args.size() != func->params.size()) throw RuntimeError(callee.line, callee.col, "Argument count mismatch.");
    
    auto newEnv = std::make_shared<Environment>(env);
    for (size_t i = 0; i < args.size(); ++i) newEnv->define(func->params[i], args[i]);
    
    try {
        for (auto& stmt : func->body) stmt->execute(newEnv);
    } catch (const ReturnException& e) {
        return e.value;
    }
    return Value();
}

void PrintStmt::execute(std::shared_ptr<Environment> env) {
    std::cout << expression->evaluate(env) << std::endl;
}

void LetStmt::execute(std::shared_ptr<Environment> env) {
    env->define(name.lexeme, initializer->evaluate(env));
}

void IfStmt::execute(std::shared_ptr<Environment> env) {
    if (condition->evaluate(env).isTruthy()) {
        for (auto& stmt : thenBranch) stmt->execute(env);
    } else {
        for (auto& stmt : elseBranch) stmt->execute(env);
    }
}

void ForStmt::execute(std::shared_ptr<Environment> env) {
    Value s = start->evaluate(env);
    Value e = end->evaluate(env);
    env->define(name.lexeme, s);
    while (env->get(name.lexeme).asNumber() <= e.asNumber()) {
        for (auto& stmt : body) stmt->execute(env);
        env->assign(name.lexeme, Value(env->get(name.lexeme).asNumber() + 1));
    }
}

void FunctionStmt::execute(std::shared_ptr<Environment> env) {
    auto func = std::make_shared<Function>();
    func->name = name.lexeme;
    for (auto& p : params) func->params.push_back(p.lexeme);
    func->body = body;
    env->defineFunction(name.lexeme, func);
}

void ReturnStmt::execute(std::shared_ptr<Environment> env) {
    Value val = value ? value->evaluate(env) : Value();
    throw ReturnException{val};
}

void InputStmt::execute(std::shared_ptr<Environment> env) {
    std::string line;
    std::getline(std::cin, line);
    try {
        env->define(name.lexeme, Value(std::stod(line)));
    } catch (...) {
        env->define(name.lexeme, Value(line));
    }
}

void TryCatchStmt::execute(std::shared_ptr<Environment> env) {
    try {
        for (auto& stmt : tryBranch) stmt->execute(env);
    } catch (const RuntimeError& e) {
        for (auto& stmt : catchBranch) stmt->execute(env);
    }
}

void ExpressionStmt::execute(std::shared_ptr<Environment> env) {
    expression->evaluate(env);
}
