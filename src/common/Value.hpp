#pragma once
#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct Value;
using Array = std::vector<std::shared_ptr<Value>>;

struct Value {
    std::variant<std::monostate, double, bool, std::string, Array> data;

    Value() : data(std::monostate{}) {}
    Value(double n) : data(n) {}
    Value(bool b) : data(b) {}
    Value(std::string s) : data(s) {}
    Value(Array a) : data(a) {}

    bool isNumber() const { return std::holds_alternative<double>(data); }
    bool isBool() const { return std::holds_alternative<bool>(data); }
    bool isString() const { return std::holds_alternative<std::string>(data); }
    bool isArray() const { return std::holds_alternative<Array>(data); }
    bool isNull() const { return std::holds_alternative<std::monostate>(data); }

    double asNumber() const { return std::get<double>(data); }
    bool asBool() const { return std::get<bool>(data); }
    std::string asString() const { return std::get<std::string>(data); }
    Array asArray() const { return std::get<Array>(data); }

    bool isTruthy() const {
        if (isNull()) return false;
        if (isBool()) return asBool();
        if (isNumber()) return asNumber() != 0;
        if (isString()) return !asString().empty();
        if (isArray()) return true;
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Value& val) {
    if (val.isNumber()) os << val.asNumber();
    else if (val.isBool()) os << (val.asBool() ? "true" : "false");
    else if (val.isString()) os << val.asString();
    else if (val.isArray()) {
        os << "[";
        auto arr = val.asArray();
        for (size_t i = 0; i < arr.size(); ++i) {
            os << *arr[i];
            if (i < arr.size() - 1) os << ", ";
        }
        os << "]";
    } else os << "null";
    return os;
}
