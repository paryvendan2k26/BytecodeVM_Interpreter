#pragma once

#include <string>

using namespace std;

enum TokenType {

    NUMBER,
    IDENTIFIER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    EQUAL,
    EQUAL_EQUAL,
    BANG_EQUAL,

    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    LPAREN,
    RPAREN,

    LBRACE,
    RBRACE,

    IF,
    ELSE,
    WHILE,
    FUNC,
    RETURN,
    PRINT,
    TRUE,
    FALSE,
    COMMA,

    EOF_TOKEN
};

class Token {
public:

    TokenType type;
    string value;
    int line;
    int column;

    Token() {
        line = 1;
        column = 1;
    }

    Token(TokenType t, string v) {
        type = t;
        value = v;
        line = 1;
        column = 1;
    }

    Token(
        TokenType t,
        string v,
        int tokenLine,
        int tokenColumn
    ) {
        type = t;
        value = v;
        line = tokenLine;
        column = tokenColumn;
    }

    string identifier();
};
