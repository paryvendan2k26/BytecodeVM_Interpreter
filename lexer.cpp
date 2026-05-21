#include "lexer.h"
#include <cctype>

using namespace std;

Lexer::Lexer(string input) {

    text = input;

    pos = 0;

    if (text.empty()) {

        currentChar = '\0';
    }

    else {

        currentChar = text[pos];
    }
}

void Lexer::advance() {

    pos++;

    if (pos >= text.size()) {

        currentChar = '\0';
    }

    else {

        currentChar = text[pos];
    }
}

vector<Token> Lexer::tokenize() {

    vector<Token> tokens;

    while (currentChar != '\0') {

        if (isdigit(currentChar)) {

            string number = "";

            while (isdigit(currentChar)) {

                number += currentChar;

                advance();
            }

            tokens.push_back(
                Token(NUMBER, number)
            );
        }

        else if (
            isalpha(currentChar) ||
            currentChar == '_'
        ) {

            string name = identifier();

            if (name == "if") {

                tokens.push_back(
                    Token(IF, "if")
                );
            }

            else if (name == "else") {

                tokens.push_back(
                    Token(ELSE, "else")
                );
            }

            else if (name == "while") {

                tokens.push_back(
                    Token(WHILE, "while")
                );
            }

            else if (name == "func") {

                tokens.push_back(
                    Token(FUNC, "func")
                );
            }

            else if (name == "return") {

                tokens.push_back(
                    Token(RETURN, "return")
                );
            }

            else if (name == "print") {

                tokens.push_back(
                    Token(PRINT, "print")
                );
            }

            else if (name == "true") {

                tokens.push_back(
                    Token(TRUE, "true")
                );
            }

            else if (name == "false") {

                tokens.push_back(
                    Token(FALSE, "false")
                );
            }

            else {

                tokens.push_back(
                    Token(IDENTIFIER, name)
                );
            }
        }

        else if (currentChar == '=') {

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    Token(EQUAL_EQUAL, "==")
                );

                advance();
            }

            else {

                tokens.push_back(
                    Token(EQUAL, "=")
                );
            }
        }

        else if (currentChar == '!') {

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    Token(BANG_EQUAL, "!=")
                );

                advance();
            }
        }

        else if (currentChar == '+') {

            tokens.push_back(
                Token(PLUS, "+")
            );

            advance();
        }

        else if (currentChar == '-') {

            tokens.push_back(
                Token(MINUS, "-")
            );

            advance();
        }

        else if (currentChar == '*') {

            tokens.push_back(
                Token(STAR, "*")
            );

            advance();
        }

        else if (currentChar == '/') {

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
                    Token(SLASH, "/")
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
                Token(LPAREN, "(")
            );

            advance();
        }

        else if (currentChar == ')') {

            tokens.push_back(
                Token(RPAREN, ")")
            );

            advance();
        }

        else if (currentChar == '{') {

            tokens.push_back(
                Token(LBRACE, "{")
            );

            advance();
        }

        else if (currentChar == '}') {

            tokens.push_back(
                Token(RBRACE, "}")
            );

            advance();
        }

        else if (currentChar == '>') {

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    Token(GREATER_EQUAL, ">=")
                );

                advance();
            }

            else {

            tokens.push_back(
                Token(GREATER, ">")
            );
            }
        }

        else if (currentChar == '<') {

            advance();

            if (currentChar == '=') {

                tokens.push_back(
                    Token(LESS_EQUAL, "<=")
                );

                advance();
            }

            else {

            tokens.push_back(
                Token(LESS, "<")
            );
            }
        }

        else if (currentChar == ',') {

            tokens.push_back(
                Token(COMMA, ",")
            );

            advance();
        }

        else {

            advance();
        }
    }

    tokens.push_back(
        Token(EOF_TOKEN, "")
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
