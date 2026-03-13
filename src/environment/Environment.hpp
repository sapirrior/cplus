#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "../common/Value.hpp"

class Stmt;

struct Function {
    std::string name;
    std::vector<std::string> params;
    std::shared_ptr<Stmt> body;
};

class Environment {
    std::unordered_map<std::string, Value> values;
    std::unordered_map<std::string, std::shared_ptr<Function>> functions;
    std::shared_ptr<Environment> enclosing;
public:
    Environment();
    Environment(std::shared_ptr<Environment> enclosing);
    
    void define(const std::string& name, const Value& value);
    void defineFunction(const std::string& name, std::shared_ptr<Function> func);
    
    Value get(const std::string& name);
    std::shared_ptr<Function> getFunction(const std::string& name);
    
    void assign(const std::string& name, const Value& value);
};
