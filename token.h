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

    Token() {
    }

    Token(TokenType t, string v) {
        type = t;
        value = v;
    }

    string identifier();
};
