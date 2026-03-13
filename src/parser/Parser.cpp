#include "Parser.hpp"
#include <iostream>

Token Parser::peek() { return tokens[current]; }
Token Parser::previous() { return tokens[current - 1]; }
bool Parser::isAtEnd() { return peek().type == TokenType::END_OF_FILE; }
Token Parser::advance() { if (!isAtEnd()) current++; return previous(); }
bool Parser::check(TokenType type) { if (isAtEnd()) return false; return peek().type == type; }
bool Parser::match(TokenType type) {
    if (check(type)) { advance(); return true; }
    return false;
}
Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw ParserError(peek().line, peek().col, message);
}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        } catch (const ParserError& e) {
            std::cerr << e.what() << std::endl;
            advance();
            while (!isAtEnd()) {
                if (previous().type == TokenType::SEMICOLON) return statements;
                switch (peek().type) {
                    case TokenType::FN: case TokenType::LET: case TokenType::FOR:
                    case TokenType::IF: case TokenType::WHILE: case TokenType::PRINT:
                    case TokenType::RETURN: return statements;
                    default: advance();
                }
            }
        }
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    if (match(TokenType::FN)) return functionDeclaration();
    if (match(TokenType::LET)) return letDeclaration();
    return statement();
}

std::shared_ptr<Stmt> Parser::functionDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect function name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before function body.");
    auto body = block();
    return std::make_shared<FunctionStmt>(name, parameters, body);
}

std::shared_ptr<Stmt> Parser::letDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match(TokenType::EQUAL)) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<LetStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::TRY)) return tryCatchStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::LEFT_BRACE)) return block();
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = statement();
    }
    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");
    std::shared_ptr<Stmt> body = statement();
    return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::forStatement() {
    throw ParserError(peek().line, peek().col, "'for' loop is currently simplified out. Use 'while'.");
}

std::shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

std::shared_ptr<Stmt> Parser::tryCatchStatement() {
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'try'.");
    std::shared_ptr<Stmt> tryBlock = block();
    consume(TokenType::CATCH, "Expect 'catch' after try block.");
    consume(TokenType::LEFT_BRACE, "Expect '{' after 'catch'.");
    std::shared_ptr<Stmt> catchBlock = block();
    return std::make_shared<TryCatchStmt>(tryBlock, catchBlock);
}

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return std::make_shared<BlockStmt>(statements);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = logical_or();

    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if (auto varExpr = std::dynamic_pointer_cast<VariableExpr>(expr)) {
            Token name = varExpr->getToken();
            return std::make_shared<AssignExpr>(name, value);
        } else if (auto arrAcc = std::dynamic_pointer_cast<ArrayAccessExpr>(expr)) {
            return std::make_shared<ArrayAssignExpr>(arrAcc->array, arrAcc->index, value);
        }
        throw ParserError(equals.line, equals.col, "Invalid assignment target.");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logical_or() {
    std::shared_ptr<Expr> expr = logical_and();
    while (match(TokenType::OR)) {
        Token op = previous();
        std::shared_ptr<Expr> right = logical_and();
        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logical_and() {
    std::shared_ptr<Expr> expr = equality();
    while (match(TokenType::AND)) {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<LogicalExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    while (match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();
    while (match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) || match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match(TokenType::MINUS) || match(TokenType::PLUS)) {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match(TokenType::SLASH) || match(TokenType::STAR)) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match(TokenType::BANG) || match(TokenType::MINUS)) {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<UnaryExpr>(op, right);
    }
    return call();
}

std::shared_ptr<Expr> Parser::call() {
    std::shared_ptr<Expr> expr = primary();

    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            std::vector<std::shared_ptr<Expr>> arguments;
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    arguments.push_back(expression());
                } while (match(TokenType::COMMA));
            }
            Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
            expr = std::make_shared<CallExpr>(expr, paren, arguments);
        } else if (match(TokenType::LEFT_BRACKET)) {
            std::shared_ptr<Expr> index = expression();
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after property access.");
            expr = std::make_shared<ArrayAccessExpr>(expr, index);
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Expr> Parser::primary() {
    if (match(TokenType::FALSE)) return std::make_shared<LiteralExpr>(Value(false));
    if (match(TokenType::TRUE)) return std::make_shared<LiteralExpr>(Value(true));
    if (match(TokenType::NULL_TOKEN)) return std::make_shared<LiteralExpr>(Value());

    if (match(TokenType::NUMBER)) return std::make_shared<LiteralExpr>(Value(previous().number_value));
    if (match(TokenType::STRING)) return std::make_shared<LiteralExpr>(Value(previous().lexeme));

    if (match(TokenType::IDENTIFIER)) return std::make_shared<VariableExpr>(previous());

    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return expr;
    }
    
    if (match(TokenType::LEFT_BRACKET)) {
        std::vector<std::shared_ptr<Expr>> elements;
        if (!check(TokenType::RIGHT_BRACKET)) {
            do {
                elements.push_back(expression());
            } while (match(TokenType::COMMA));
        }
        consume(TokenType::RIGHT_BRACKET, "Expect ']' after array elements.");
        return std::make_shared<ArrayLiteralExpr>(elements);
    }

    throw ParserError(peek().line, peek().col, "Expect expression.");
}
