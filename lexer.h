#pragma once

#include "token.h"
#include <vector>

using namespace std;

class Lexer {
public:
    string text;
    size_t pos;
    char currentChar;
    int line;
    int column;

    Lexer(string input);

    void advance();

    Token makeToken(
        TokenType type,
        string value,
        int tokenColumn
    );

    vector<Token> tokenize();
    string identifier();
};
