#include "Environment.hpp"

Environment::Environment() : enclosing(nullptr) {}
Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

void Environment::define(const std::string& name, const Value& value) {
    values[name] = value;
}

void Environment::defineFunction(const std::string& name, std::shared_ptr<Function> func) {
    functions[name] = func;
}

Value Environment::get(const std::string& name) {
    if (values.find(name) != values.end()) return values[name];
    if (enclosing) return enclosing->get(name);
    throw std::runtime_error("Undefined variable '" + name + "'.");
}

std::shared_ptr<Function> Environment::getFunction(const std::string& name) {
    if (functions.find(name) != functions.end()) return functions[name];
    if (enclosing) return enclosing->getFunction(name);
    return nullptr;
}

void Environment::assign(const std::string& name, const Value& value) {
    if (values.find(name) != values.end()) {
        values[name] = value;
        return;
    }
    if (enclosing) {
        enclosing->assign(name, value);
        return;
    }
    throw std::runtime_error("Undefined variable '" + name + "'.");
}
