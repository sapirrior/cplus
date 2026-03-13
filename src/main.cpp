#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "parser/AST.hpp"
#include "core/Environment.hpp"

std::string parseVersionFromConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "unknown";
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    size_t pos = content.find("\"version\"");
    if (pos != std::string::npos) {
        size_t colon = content.find(":", pos);
        if (colon != std::string::npos) {
            size_t quote1 = content.find("\"", colon);
            if (quote1 != std::string::npos) {
                size_t quote2 = content.find("\"", quote1 + 1);
                if (quote2 != std::string::npos) {
                    return content.substr(quote1 + 1, quote2 - quote1 - 1);
                }
            }
        }
    }
    return "unknown";
}

void run(const std::string& source, std::shared_ptr<Environment> env) {
    try {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();
        
        Parser parser(tokens);
        std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
        
        for (const auto& stmt : statements) {
            stmt->execute(env);
        }
    } catch (const ParserError& e) {
        std::cerr << e.what() << std::endl;
    } catch (const RuntimeError& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred during execution." << std::endl;
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
    std::cout << "C+ REPL (C++20). Type 'exit' to quit." << std::endl;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        run(line + "\n", env);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: cplus [script.cp] | -version | -help" << std::endl;
        return 64;
    } else if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-version") {
            std::cout << "C+ Version: " << parseVersionFromConfig("config.json") << std::endl;
        } else if (arg == "-help") {
            std::cout << "Usage: cplus [script.cp]" << std::endl;
            std::cout << "  -version   Print the version" << std::endl;
            std::cout << "  -help      Print this help message" << std::endl;
        } else {
            runFile(arg);
        }
    } else {
        runPrompt();
    }
    return 0;
}
