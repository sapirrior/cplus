#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "ast/Stmt.hpp"
#include "environment/Environment.hpp"

void run(const std::string& source, std::shared_ptr<Environment> env) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    
    for (const auto& stmt : statements) {
        try {
            stmt->execute(env);
        } catch (const RuntimeError& e) {
            std::cerr << e.what() << std::endl;
        } catch (...) {
            std::cerr << "An unknown error occurred during execution." << std::endl;
        }
    }
}

void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto env = std::make_shared<Environment>();
    run(source, env);
}

void runPrompt() {
    auto env = std::make_shared<Environment>();
    std::string line;
    std::cout << "C+ JS-Style REPL (C++20). Type 'exit' to quit." << std::endl;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        run(line + "\n", env);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: cplus [script.cp]" << std::endl;
        return 64;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
    return 0;
}
