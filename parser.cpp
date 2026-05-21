#include "parser.h"

#include <stdexcept>

using namespace std;

Parser::Parser(vector<Token> t) {

    tokens = t;

    pos = 0;

    currentToken = tokens[pos];
}

void Parser::advance() {

    pos++;

    if (pos < tokens.size()) {

        currentToken = tokens[pos];
    }
}

bool Parser::check(TokenType type) {

    return currentToken.type == type;
}

string Parser::tokenName(TokenType type) {

    switch (type) {

        case NUMBER: return "number";
        case IDENTIFIER: return "identifier";
        case PLUS: return "+";
        case MINUS: return "-";
        case STAR: return "*";
        case SLASH: return "/";
        case EQUAL: return "=";
        case EQUAL_EQUAL: return "==";
        case BANG_EQUAL: return "!=";
        case GREATER: return ">";
        case GREATER_EQUAL: return ">=";
        case LESS: return "<";
        case LESS_EQUAL: return "<=";
        case LPAREN: return "(";
        case RPAREN: return ")";
        case LBRACE: return "{";
        case RBRACE: return "}";
        case IF: return "if";
        case ELSE: return "else";
        case WHILE: return "while";
        case FUNC: return "func";
        case RETURN: return "return";
        case PRINT: return "print";
        case TRUE: return "true";
        case FALSE: return "false";
        case COMMA: return ",";
        case EOF_TOKEN: return "end of input";
    }

    return "token";
}

void Parser::parserError(string expected) {

    string found =
        tokenName(currentToken.type);

    if (!currentToken.value.empty()) {

        found += " '" + currentToken.value + "'";
    }

    throw runtime_error(
        "Parser error at line " +
        to_string(currentToken.line) +
        ", column " +
        to_string(currentToken.column) +
        ": expected " +
        expected +
        ", found " +
        found
    );
}

Token Parser::expect(
    TokenType type,
    string expected
) {

    if (!check(type)) {

        parserError(expected);
    }

    Token token = currentToken;

    advance();

    return token;
}

AST* Parser::factor() {

    if (currentToken.type == NUMBER) {

        AST* node =
            new NumberNode(currentToken.value);

        advance();

        return node;
    }

    else if (
        currentToken.type == TRUE ||
        currentToken.type == FALSE
    ) {

        AST* node =
            new BooleanNode(
                currentToken.type == TRUE
            );

        advance();

        return node;
    }

    else if (
        currentToken.type ==
        IDENTIFIER
    ) {

        if (
            pos + 1 < tokens.size() &&
            tokens[pos + 1].type == LPAREN
        ) {

            return functionCall();
        }

        AST* node =
            new VariableNode(
                currentToken.value
            );

        advance();

        return node;
    }

    else if (currentToken.type == LPAREN) {

        advance();

        AST* node = comparison();

        expect(RPAREN, "')'");

        return node;
    }

    parserError(
        "expression"
    );

    return nullptr;
}

AST* Parser::unary() {

    if (currentToken.type == MINUS) {

        string op = currentToken.value;

        advance();

        return new UnaryOpNode(
            op,
            unary()
        );
    }

    return factor();
}

AST* Parser::term() {

    AST* node = unary();

    while (
        currentToken.type == STAR ||
        currentToken.type == SLASH
    ) {

        string op = currentToken.value;

        advance();

        AST* right = unary();

        node =
            new BinaryOpNode(node, op, right);
    }

    return node;
}

AST* Parser::expr() {

    AST* node = term();

    while (
        currentToken.type == PLUS ||
        currentToken.type == MINUS
    ) {

        string op = currentToken.value;

        advance();

        AST* right = term();

        node =
            new BinaryOpNode(node, op, right);
    }

    return node;
}

AST* Parser::comparison() {

    AST* node = expr();

    while (

        currentToken.type == GREATER ||

        currentToken.type == GREATER_EQUAL ||

        currentToken.type == LESS ||

        currentToken.type == LESS_EQUAL ||

        currentToken.type == BANG_EQUAL ||

        currentToken.type == EQUAL_EQUAL
    ) {

        string op =
            currentToken.value;

        advance();

        AST* right = expr();

        node = new CompareNode(
            node,
            op,
            right
        );
    }

    return node;
}

AST* Parser::ifStatement() {

    advance();

    AST* condition =
        comparison();

    expect(LBRACE, "'{' after if condition");

    vector<AST*> ifBody;

    while (
        currentToken.type != RBRACE &&
        currentToken.type != EOF_TOKEN
    ) {

        ifBody.push_back(
            statement()
        );
    }

    expect(RBRACE, "'}' after if body");

    vector<AST*> elseBody;

    if (
        currentToken.type ==
        ELSE
    ) {

        advance();

        expect(LBRACE, "'{' after else");

        while (
            currentToken.type != RBRACE &&
            currentToken.type != EOF_TOKEN
        ) {

            elseBody.push_back(
                statement()
            );
        }

        expect(RBRACE, "'}' after else body");
    }

    return new IfNode(
        condition,
        ifBody,
        elseBody
    );
}
AST* Parser::statement() {

    if (currentToken.type == FUNC) {

        return functionDefinition();
    }

    if (currentToken.type == IF) {

        return ifStatement();
    }

    if (currentToken.type == WHILE) {

        return whileStatement();
    }

    if (currentToken.type == RETURN) {

        return returnStatement();
    }

    if (currentToken.type == PRINT) {

        return printStatement();
    }

    if (
        currentToken.type == IDENTIFIER &&
        pos + 1 < tokens.size() &&
        tokens[pos + 1].type == EQUAL
    ) {

        string name = currentToken.value;

        advance();

        advance();

        AST* value = comparison();

        return new AssignNode(name, value);
    }

    return comparison();
}

AST* Parser::whileStatement() {

    advance();

    AST* condition =
        comparison();

    expect(LBRACE, "'{' after while condition");

    vector<AST*> body;

    while (
        currentToken.type != RBRACE &&
        currentToken.type != EOF_TOKEN
    ) {

        body.push_back(
            statement()
        );
    }

    expect(RBRACE, "'}' after while body");

    return new WhileNode(
        condition,
        body
    );
}

AST* Parser::program() {

    vector<AST*> statements;

    while (
        currentToken.type != EOF_TOKEN
    ) {

        statements.push_back(
            statement()
        );
    }

    return new ProgramNode(statements);
}

AST* Parser::functionDefinition() {

    advance();

    Token nameToken =
        expect(IDENTIFIER, "function name");

    string name =
        nameToken.value;

    expect(LPAREN, "'(' after function name");

    vector<string> params;

    while (
        currentToken.type != RPAREN
    ) {

        Token paramToken =
            expect(IDENTIFIER, "parameter name");

        params.push_back(
            paramToken.value
        );

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }

        else if (
            currentToken.type != RPAREN
        ) {

            parserError("',' or ')' after parameter");
        }
    }

    expect(RPAREN, "')' after function parameters");

    expect(LBRACE, "'{' before function body");

    vector<AST*> body;

    while (
        currentToken.type != RBRACE &&
        currentToken.type != EOF_TOKEN
    ) {

        body.push_back(
            statement()
        );
    }

    expect(RBRACE, "'}' after function body");

    return new FunctionDefNode(
        name,
        params,
        body
    );
}

AST* Parser::functionCall() {

    string name =
        currentToken.value;

    advance();

    expect(LPAREN, "'(' after function name");

    vector<AST*> args;

    while (
        currentToken.type != RPAREN
    ) {

        args.push_back(
            comparison()
        );

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }

        else if (
            currentToken.type != RPAREN
        ) {

            parserError("',' or ')' after argument");
        }
    }

    expect(RPAREN, "')' after function arguments");

    return new FunctionCallNode(
        name,
        args
    );
}

AST* Parser::returnStatement() {

    advance();

    if (
        currentToken.type == RBRACE ||
        currentToken.type == EOF_TOKEN
    ) {

        parserError("expression after return");
    }

    AST* value =
        comparison();

    return new ReturnNode(value);
}

AST* Parser::printStatement() {

    advance();

    expect(LPAREN, "'(' after print");

    AST* value =
        comparison();

    expect(RPAREN, "')' after print expression");

    return new PrintNode(value);
}


AST* Parser::parse() {

    return program();
}
