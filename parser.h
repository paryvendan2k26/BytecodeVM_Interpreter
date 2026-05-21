#pragma once

#include "token.h"
#include "ast.h"
#include <vector>

using namespace std;

class Parser {

public:

    vector<Token> tokens;

    size_t pos;

    Token currentToken;

    Parser(vector<Token> t);

    void advance();

    AST* parse();

    AST* expr();

    AST* term();

    AST* factor();

    AST* unary();

    AST* statement();

    AST* program();

    AST* ifStatement();

    AST* comparison();

    AST* whileStatement();

    AST* functionDefinition();

    AST* functionCall();

    AST* returnStatement();

    AST* printStatement();
};
