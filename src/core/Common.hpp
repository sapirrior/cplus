#pragma once
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <iostream>
#include <stdexcept>

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET,
    COMMA, DOT, MINUS, PLUS, SLASH, STAR,

    // One or two character tokens.
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    BANG_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, OR, NOT,
    PRINT, LET, IF, THEN, ELSE, END, FOR, TO, NEXT,
    FUNCTION, CALL, RETURN,
    INPUT, REM, TRY, CATCH,
    ARRAY,

    NEWLINE, END_OF_FILE, ERROR_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    double number_value = 0;
    int line = 1;
    int col = 1;
};

struct Value;
using Array = std::vector<std::shared_ptr<Value>>;

struct Value {
    std::variant<std::monostate, double, std::string, Array> data;

    Value() : data(std::monostate{}) {}
    Value(double n) : data(n) {}
    Value(std::string s) : data(s) {}
    Value(Array a) : data(a) {}

    bool isNumber() const { return std::holds_alternative<double>(data); }
    bool isString() const { return std::holds_alternative<std::string>(data); }
    bool isArray() const { return std::holds_alternative<Array>(data); }
    bool isNil() const { return std::holds_alternative<std::monostate>(data); }

    double asNumber() const { return std::get<double>(data); }
    std::string asString() const { return std::get<std::string>(data); }
    Array asArray() const { return std::get<Array>(data); }

    bool isTruthy() const {
        if (isNil()) return false;
        if (isNumber()) return asNumber() != 0;
        if (isString()) return !asString().empty();
        if (isArray()) return !asArray().empty();
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Value& val) {
    if (val.isNumber()) os << val.asNumber();
    else if (val.isString()) os << val.asString();
    else if (val.isArray()) {
        os << "[";
        auto arr = val.asArray();
        for (size_t i = 0; i < arr.size(); ++i) {
            os << *arr[i];
            if (i < arr.size() - 1) os << ", ";
        }
        os << "]";
    } else os << "nil";
    return os;
}

class ParserError : public std::runtime_error {
public:
    int line, col;
    ParserError(int line, int col, const std::string& message) 
        : std::runtime_error("SyntaxError: " + message + " (Line " + std::to_string(line) + ", Col " + std::to_string(col) + ")"), line(line), col(col) {}
};

class RuntimeError : public std::runtime_error {
public:
    int line, col;
    RuntimeError(int line, int col, const std::string& message) 
        : std::runtime_error("RuntimeError: " + message + " (Line " + std::to_string(line) + ", Col " + std::to_string(col) + ")"), line(line), col(col) {}
};
