#include "Parser.hpp"

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        try {
            statements.push_back(statement());
        } catch (const ParserError& e) {
            std::cerr << e.what() << std::endl;
            while (!isAtEnd() && peek().type != TokenType::NEWLINE) advance();
        }
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::LET)) return letStatement();
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::FUNCTION)) return functionStatement();
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::INPUT)) return inputStatement();
    if (match(TokenType::TRY)) return tryCatchStatement();
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> expr = expression();
    if (!isAtEnd()) match(TokenType::NEWLINE);
    return std::make_shared<PrintStmt>(expr);
}

std::shared_ptr<Stmt> Parser::letStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    consume(TokenType::EQUAL, "Expect '=' after variable name.");
    std::shared_ptr<Expr> initializer = expression();
    if (!isAtEnd()) match(TokenType::NEWLINE);
    return std::make_shared<LetStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::THEN, "Expect 'THEN' after IF condition.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    
    std::vector<std::shared_ptr<Stmt>> thenBranch;
    while (!check(TokenType::ELSE) && !check(TokenType::END) && !isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        thenBranch.push_back(statement());
    }
    
    std::vector<std::shared_ptr<Stmt>> elseBranch;
    if (match(TokenType::ELSE)) {
        if (!isAtEnd()) match(TokenType::NEWLINE);
        while (!check(TokenType::END) && !isAtEnd()) {
            if (match(TokenType::NEWLINE)) continue;
            elseBranch.push_back(statement());
        }
    }
    
    consume(TokenType::END, "Expect 'END' after IF block.");
    consume(TokenType::IF, "Expect 'IF' after END.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    
    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::forStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    consume(TokenType::EQUAL, "Expect '=' after variable name.");
    std::shared_ptr<Expr> start = expression();
    consume(TokenType::TO, "Expect 'TO' after start.");
    std::shared_ptr<Expr> end = expression();
    if (!isAtEnd()) match(TokenType::NEWLINE);
    
    std::vector<std::shared_ptr<Stmt>> body;
    while (!check(TokenType::NEXT) && !isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        body.push_back(statement());
    }
    consume(TokenType::NEXT, "Expect 'NEXT' after FOR block.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    
    return std::make_shared<ForStmt>(name, start, end, body);
}

std::shared_ptr<Stmt> Parser::functionStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect function name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    
    std::vector<std::shared_ptr<Stmt>> body;
    while (!check(TokenType::END) && !isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        body.push_back(statement());
    }
    consume(TokenType::END, "Expect 'END' after function body.");
    consume(TokenType::FUNCTION, "Expect 'FUNCTION' after END.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    
    return std::make_shared<FunctionStmt>(name, parameters, body);
}

std::shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (!check(TokenType::NEWLINE) && !check(TokenType::END_OF_FILE)) {
        value = expression();
    }
    if (!isAtEnd()) match(TokenType::NEWLINE);
    return std::make_shared<ReturnStmt>(keyword, value);
}

std::shared_ptr<Stmt> Parser::inputStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name for INPUT.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    return std::make_shared<InputStmt>(name);
}

std::shared_ptr<Stmt> Parser::tryCatchStatement() {
    if (!isAtEnd()) match(TokenType::NEWLINE);
    std::vector<std::shared_ptr<Stmt>> tryBranch;
    while (!check(TokenType::CATCH) && !isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        tryBranch.push_back(statement());
    }
    consume(TokenType::CATCH, "Expect 'CATCH' after TRY block.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    std::vector<std::shared_ptr<Stmt>> catchBranch;
    while (!check(TokenType::END) && !isAtEnd()) {
        if (match(TokenType::NEWLINE)) continue;
        catchBranch.push_back(statement());
    }
    consume(TokenType::END, "Expect 'END' after CATCH block.");
    consume(TokenType::TRY, "Expect 'TRY' after END.");
    if (!isAtEnd()) match(TokenType::NEWLINE);
    return std::make_shared<TryCatchStmt>(tryBranch, catchBranch);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    if (!isAtEnd()) match(TokenType::NEWLINE);
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return logical_or();
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
    while (match(TokenType::EQUAL_EQUAL) || match(TokenType::BANG_EQUAL)) {
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
    if (match(TokenType::NOT) || match(TokenType::MINUS)) {
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
            consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
            if (auto varExpr = std::dynamic_pointer_cast<VariableExpr>(expr)) {
                expr = std::make_shared<CallExpr>(varExpr->getToken(), arguments);
            } else {
                throw ParserError(peek().line, peek().col, "Only variables can be called.");
            }
        } else if (match(TokenType::LEFT_BRACKET)) {
            std::shared_ptr<Expr> index = expression();
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after index.");
            expr = std::make_shared<ArrayAccessExpr>(expr, index);
        } else {
            break;
        }
    }
    return expr;
}

std::shared_ptr<Expr> Parser::primary() {
    if (match(TokenType::NUMBER)) return std::make_shared<LiteralExpr>(Value(previous().number_value));
    if (match(TokenType::STRING)) return std::make_shared<LiteralExpr>(Value(previous().lexeme));
    if (match(TokenType::IDENTIFIER)) return std::make_shared<VariableExpr>(previous());
    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<GroupingExpr>(expr);
    }
    throw ParserError(peek().line, peek().col, "Expect expression.");
}
