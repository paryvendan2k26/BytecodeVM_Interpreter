#include "parser.h"

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

        if (currentToken.type == RPAREN) {

            advance();
        }

        return node;
    }

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

    if (
        currentToken.type ==
        LBRACE
    ) {

        advance();
    }

    vector<AST*> ifBody;

    while (
        currentToken.type !=
        RBRACE
    ) {

        ifBody.push_back(
            statement()
        );
    }

    advance();

    vector<AST*> elseBody;

    if (
        currentToken.type ==
        ELSE
    ) {

        advance();

        if (
            currentToken.type ==
            LBRACE
        ) {

            advance();
        }

        while (
            currentToken.type !=
            RBRACE
        ) {

            elseBody.push_back(
                statement()
            );
        }

        advance();
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

    if (currentToken.type == LBRACE) {

        advance();
    }

    vector<AST*> body;

    while (
        currentToken.type != RBRACE
    ) {

        body.push_back(
            statement()
        );
    }

    advance();

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

    string name =
        currentToken.value;

    advance();

    advance();

    vector<string> params;

    while (
        currentToken.type != RPAREN
    ) {

        params.push_back(
            currentToken.value
        );

        advance();

        if (
            currentToken.type == COMMA
        ) {

            advance();
        }
    }

    advance();

    if (
        currentToken.type == LBRACE
    ) {

        advance();
    }

    vector<AST*> body;

    while (
        currentToken.type != RBRACE
    ) {

        body.push_back(
            statement()
        );
    }

    advance();

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

    advance();

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
    }

    advance();

    return new FunctionCallNode(
        name,
        args
    );
}

AST* Parser::returnStatement() {

    advance();

    AST* value =
        comparison();

    return new ReturnNode(value);
}

AST* Parser::printStatement() {

    advance();

    if (currentToken.type == LPAREN) {

        advance();
    }

    AST* value =
        comparison();

    if (currentToken.type == RPAREN) {

        advance();
    }

    return new PrintNode(value);
}


AST* Parser::parse() {

    return program();
}
