# C+ Programming Language

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)

C+ is a modern, production-grade BASIC-style interpreter written in C++17. It's designed to be fast, simple, and extensible.

## Features
- **Modern Syntax**: Clean, BASIC-inspired syntax without line numbers.
- **Robust Execution**: AST-based execution with smart pointer-based memory management.
- **Advanced CLI**: Includes a REPL, script execution, and versioning.
- **Detailed Error Reporting**: Comprehensive syntax and runtime error tracking.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Language Guide](#language-guide)
- [Examples](#examples)
- [Documentation](docs/01-introduction.md)
- [License](#license)

## Installation

### From Source
You'll need a C++17 compatible compiler and CMake (optional but recommended).

```bash
git clone https://github.com/sapirrior/cplus.git
cd cplus
clang++ -std=c++17 src/main.cpp src/core/*.cpp src/lexer/*.cpp src/parser/*.cpp src/interpreter/*.cpp -I src/ -o cplus
```

## Usage

### Run a Script
```bash
./cplus examples/hello_world.cp
```

### Interactive REPL
```bash
./cplus
```

### CLI Options
- `-version`: Print the current version.
- `-help`: Print usage instructions.

## Language Guide

### Variables
Use `LET` to define variables.
```basic
LET x = 10
LET name = "C+"
```

### Control Flow
Supports `IF/THEN/END IF` and `FOR/TO/NEXT` loops.
```basic
IF x > 5 THEN
    PRINT "High"
END IF

FOR i = 1 TO 5
    PRINT i
NEXT
```

## Examples
See the [examples/](examples/) directory for more complex scripts like:
- `hello_world.cp`
- `loops.cp`
- `fibonacci.cp`

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
