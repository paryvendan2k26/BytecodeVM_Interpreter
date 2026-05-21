#pragma once

#include "token.h"
#include <vector>

using namespace std;

class Lexer {
public:
    string text;
    size_t pos;
    char currentChar;

    Lexer(string input);

    void advance();

    vector<Token> tokenize();
    string identifier();
};
