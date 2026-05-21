#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <string>

using namespace std;

Lexer::Lexer(string input) {

    text = input;

    pos = 0;
    line = 1;
    column = 1;

    if (text.empty()) {

        currentChar = '\0';
    }

    else {

        currentChar = text[pos];
    }
}

void Lexer::advance() {

    if (currentChar == '\n') {

        line++;
        column = 1;
    }

    else {

        column++;
    }

    pos++;

    if (pos >= text.size()) {

        currentChar = '\0';
    }

    else {

        currentChar = text[pos];
    }
}

Token Lexer::makeToken(
    TokenType type,
    string value,
    int tokenColumn
) {

    return Token(
        type,
        value,
        line,
        tokenColumn
    );
}

vector<Token> Lexer::tokenize() {

    vector<Token> tokens;

    while (currentChar != '\0') {

        if (isdigit(currentChar)) {

            string number = "";
            int tokenColumn = column;

            while (isdigit(currentChar)) {

                number += currentChar;

                advance();
            }

            tokens.push_back(
                makeToken(
                    NUMBER,
                    number,
                    tokenColumn
                )
            );
        }

        else if (
            isalpha(currentChar) ||
            currentChar == '_'
        ) {

            int tokenColumn = column;
            string name = identifier();

            if (name == "if") {

                tokens.push_back(
                    makeToken(IF, "if", tokenColumn)
                );
            }

            else if (name == "else") {

                tokens.push_back(
                    makeToken(ELSE, "else", tokenColumn)
                );
            }

            else if (name == "while") {

                tokens.push_back(
                    makeToken(WHILE, "while", tokenColumn)
                );
            }

            else if (name == "func") {

                tokens.push_back(
                    makeToken(FUNC, "func", tokenColumn)
                );
            }

            else if (name == "return") {

                tokens.push_back(
                    makeToken(RETURN, "return", tokenColumn)
                );
            }

            else if (name == "print") {

                tokens.push_back(
                    makeToken(PRINT, "print", tokenColumn)
                );
            }

            else if (name == "true") {

                tokens.push_back(
                    makeToken(TRUE, "true", tokenColumn)
                );
            }

            else if (name == "false") {

                tokens.push_back(
                    makeToken(FALSE, "false", tokenColumn)
                );
            }

            else {

                tokens.push_back(
                    makeToken(IDENTIFIER, name, tokenColumn)
                );
            }
        }

        else if (currentChar == '=') {

            int tokenColumn = column;

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    makeToken(EQUAL_EQUAL, "==", tokenColumn)
                );

                advance();
            }

            else {

                tokens.push_back(
                    makeToken(EQUAL, "=", tokenColumn)
                );
            }
        }

        else if (currentChar == '!') {

            int tokenColumn = column;

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    makeToken(BANG_EQUAL, "!=", tokenColumn)
                );

                advance();
            }

            else {

                throw runtime_error(
                    "Lexer error at line " +
                    to_string(line) +
                    ", column " +
                    to_string(tokenColumn) +
                    ": unexpected character '!'"
                );
            }
        }

        else if (currentChar == '+') {

            tokens.push_back(
                makeToken(PLUS, "+", column)
            );

            advance();
        }

        else if (currentChar == '-') {

            tokens.push_back(
                makeToken(MINUS, "-", column)
            );

            advance();
        }

        else if (currentChar == '*') {

            tokens.push_back(
                makeToken(STAR, "*", column)
            );

            advance();
        }

        else if (currentChar == '/') {

            int tokenColumn = column;

            advance();

            if (currentChar == '/') {

                while (
                    currentChar != '\n' &&
                    currentChar != '\0'
                ) {

                    advance();
                }
            }

            else {

                tokens.push_back(
                    makeToken(SLASH, "/", tokenColumn)
                );
            }
        }

        else if (currentChar == '#') {

            while (
                currentChar != '\n' &&
                currentChar != '\0'
            ) {

                advance();
            }
        }

        else if (currentChar == '(') {

            tokens.push_back(
                makeToken(LPAREN, "(", column)
            );

            advance();
        }

        else if (currentChar == ')') {

            tokens.push_back(
                makeToken(RPAREN, ")", column)
            );

            advance();
        }

        else if (currentChar == '{') {

            tokens.push_back(
                makeToken(LBRACE, "{", column)
            );

            advance();
        }

        else if (currentChar == '}') {

            tokens.push_back(
                makeToken(RBRACE, "}", column)
            );

            advance();
        }

        else if (currentChar == '>') {

            int tokenColumn = column;

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    makeToken(GREATER_EQUAL, ">=", tokenColumn)
                );

                advance();
            }

            else {

            tokens.push_back(
                makeToken(GREATER, ">", tokenColumn)
            );
            }
        }

        else if (currentChar == '<') {

            int tokenColumn = column;

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    makeToken(LESS_EQUAL, "<=", tokenColumn)
                );

                advance();
            }

            else {

            tokens.push_back(
                makeToken(LESS, "<", tokenColumn)
            );
            }
        }

        else if (currentChar == ',') {

            tokens.push_back(
                makeToken(COMMA, ",", column)
            );

            advance();
        }

        else if (isspace(currentChar)) {

            advance();
        }

        else {

            char unexpected = currentChar;
            int tokenColumn = column;

            throw runtime_error(
                "Lexer error at line " +
                to_string(line) +
                ", column " +
                to_string(tokenColumn) +
                ": unknown character '" +
                string(1, unexpected) +
                "'"
            );
        }
    }

    tokens.push_back(
        makeToken(EOF_TOKEN, "", column)
    );

    return tokens;
}

string Lexer::identifier() {

    string result = "";

    while (
        isalnum(currentChar) ||
        currentChar == '_'
    ) {

        result += currentChar;

        advance();
    }

    return result;
}
