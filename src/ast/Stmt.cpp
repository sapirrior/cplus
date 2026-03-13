#include "Stmt.hpp"
#include "../environment/Environment.hpp"
#include "../error/Exceptions.hpp"
#include <iostream>

void BlockStmt::execute(std::shared_ptr<Environment> env) {
    auto newEnv = std::make_shared<Environment>(env);
    for (auto& stmt : statements) {
        stmt->execute(newEnv);
    }
}

void ExpressionStmt::execute(std::shared_ptr<Environment> env) {
    expression->evaluate(env);
}

void LetStmt::execute(std::shared_ptr<Environment> env) {
    Value val = initializer ? initializer->evaluate(env) : Value();
    env->define(name.lexeme, val);
}

void IfStmt::execute(std::shared_ptr<Environment> env) {
    if (condition->evaluate(env).isTruthy()) {
        thenBranch->execute(env);
    } else if (elseBranch) {
        elseBranch->execute(env);
    }
}

void WhileStmt::execute(std::shared_ptr<Environment> env) {
    while (condition->evaluate(env).isTruthy()) {
        body->execute(env);
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

void PrintStmt::execute(std::shared_ptr<Environment> env) {
    std::cout << expression->evaluate(env) << std::endl;
}

void TryCatchStmt::execute(std::shared_ptr<Environment> env) {
    try {
        tryBlock->execute(env);
    } catch (const RuntimeError& e) {
        catchBlock->execute(env);
    }
}
