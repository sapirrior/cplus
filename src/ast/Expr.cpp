#include "Expr.hpp"
#include "Stmt.hpp"
#include "../environment/Environment.hpp"
#include "../error/Exceptions.hpp"
#include <cmath>

Value BinaryExpr::evaluate(std::shared_ptr<Environment> env) {
    Value l = left->evaluate(env);
    Value r = right->evaluate(env);
    switch (op.type) {
        case TokenType::PLUS:
            if (l.isNumber() && r.isNumber()) return Value(l.asNumber() + r.asNumber());
            if (l.isString() && r.isString()) return Value(l.asString() + r.asString());
            throw RuntimeError(op.line, op.col, "Operands must be two numbers or two strings.");
        case TokenType::MINUS: return Value(l.asNumber() - r.asNumber());
        case TokenType::STAR: return Value(l.asNumber() * r.asNumber());
        case TokenType::SLASH:
            if (r.asNumber() == 0) throw RuntimeError(op.line, op.col, "Division by zero.");
            return Value(l.asNumber() / r.asNumber());
        case TokenType::GREATER: return Value(l.asNumber() > r.asNumber());
        case TokenType::LESS: return Value(l.asNumber() < r.asNumber());
        case TokenType::GREATER_EQUAL: return Value(l.asNumber() >= r.asNumber());
        case TokenType::LESS_EQUAL: return Value(l.asNumber() <= r.asNumber());
        case TokenType::EQUAL_EQUAL:
            if (l.isNumber() && r.isNumber()) return Value(l.asNumber() == r.asNumber());
            if (l.isString() && r.isString()) return Value(l.asString() == r.asString());
            if (l.isBool() && r.isBool()) return Value(l.asBool() == r.asBool());
            if (l.isNull() && r.isNull()) return Value(true);
            return Value(false);
        case TokenType::BANG_EQUAL:
            if (l.isNumber() && r.isNumber()) return Value(l.asNumber() != r.asNumber());
            if (l.isString() && r.isString()) return Value(l.asString() != r.asString());
            if (l.isBool() && r.isBool()) return Value(l.asBool() != r.asBool());
            if (l.isNull() && r.isNull()) return Value(false);
            return Value(true);
        default: break;
    }
    throw RuntimeError(op.line, op.col, "Unknown binary operator.");
}

Value LogicalExpr::evaluate(std::shared_ptr<Environment> env) {
    Value l = left->evaluate(env);
    if (op.type == TokenType::OR) { if (l.isTruthy()) return l; }
    else { if (!l.isTruthy()) return l; }
    return right->evaluate(env);
}

Value UnaryExpr::evaluate(std::shared_ptr<Environment> env) {
    Value r = right->evaluate(env);
    if (op.type == TokenType::BANG) return Value(!r.isTruthy());
    if (op.type == TokenType::MINUS) return Value(-r.asNumber());
    return Value();
}

Value VariableExpr::evaluate(std::shared_ptr<Environment> env) {
    return env->get(name.lexeme);
}

Value AssignExpr::evaluate(std::shared_ptr<Environment> env) {
    Value val = value->evaluate(env);
    env->assign(name.lexeme, val);
    return val;
}

Value ArrayAssignExpr::evaluate(std::shared_ptr<Environment> env) {
    Value arrVal = array->evaluate(env);
    Value idxVal = index->evaluate(env);
    Value v = value->evaluate(env);
    if (!arrVal.isArray()) throw RuntimeError(0, 0, "Target is not an array.");
    if (!idxVal.isNumber()) throw RuntimeError(0, 0, "Index is not a number.");
    auto arr = arrVal.asArray();
    size_t i = (size_t)idxVal.asNumber();
    if (i >= arr.size()) throw RuntimeError(0, 0, "Array index out of bounds.");
    *arr[i] = v;
    return v;
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

    if (auto varExpr = std::dynamic_pointer_cast<VariableExpr>(callee)) {
        auto func = env->getFunction(varExpr->getToken().lexeme);
        if (func == nullptr) {
            std::string name = varExpr->getToken().lexeme;
            if (name == "len" && args.size() == 1) {
                if (args[0].isString()) return Value((double)args[0].asString().length());
                if (args[0].isArray()) return Value((double)args[0].asArray().size());
            }
            throw RuntimeError(paren.line, paren.col, "Undefined function '" + name + "'.");
        }
        
        if (args.size() != func->params.size()) throw RuntimeError(paren.line, paren.col, "Argument count mismatch.");
        
        auto newEnv = std::make_shared<Environment>(env);
        for (size_t i = 0; i < args.size(); ++i) newEnv->define(func->params[i], args[i]);
        
        try {
            func->body->execute(newEnv);
        } catch (const ReturnException& e) {
            return e.value;
        }
        return Value();
    }
    throw RuntimeError(paren.line, paren.col, "Can only call functions.");
}

Value ArrayLiteralExpr::evaluate(std::shared_ptr<Environment> env) {
    Array arr;
    for (auto& el : elements) {
        arr.push_back(std::make_shared<Value>(el->evaluate(env)));
    }
    return Value(arr);
}
