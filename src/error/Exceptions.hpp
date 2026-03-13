#pragma once
#include <stdexcept>
#include <string>
#include "../common/Value.hpp"

class ParserError : public std::runtime_error {
public:
    int line, col;
    ParserError(int line, int col, const std::string& message) 
        : std::runtime_error("SyntaxError: " + message + " (Line " + std::to_string(line) + ":" + std::to_string(col) + ")"), line(line), col(col) {}
};

class RuntimeError : public std::runtime_error {
public:
    int line, col;
    RuntimeError(int line, int col, const std::string& message) 
        : std::runtime_error("RuntimeError: " + message + " (Line " + std::to_string(line) + ":" + std::to_string(col) + ")"), line(line), col(col) {}
};

struct ReturnException { Value value; };
