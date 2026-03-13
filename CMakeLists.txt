cmake_minimum_required(VERSION 3.10)
project(cplus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set common include directories
include_directories(src)

add_executable(cplus
    src/main.cpp
    src/core/Environment.cpp
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/interpreter/Interpreter.cpp
)
