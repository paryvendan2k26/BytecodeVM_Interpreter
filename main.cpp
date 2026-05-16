#include <iostream>

#include "lexer.h"
#include "parser.h"

#include "compiler.h"
#include "vm.h"

using namespace std;

int main() {

string input =

"fact(5) "

"func fact(n) { "

"if n == 0 { "
"1 "
"} "

"else { "
"n * fact(n - 1) "
"} "

"}";
    Lexer lexer(input);

    vector<Token> tokens =
        lexer.tokenize();

    Parser parser(tokens);

    AST* tree =
        parser.parse();

    Compiler compiler;

    compiler.compile(tree);

    VM vm;

    vm.functions =
        compiler.functions;

    int result =
        vm.run(
            compiler.instructions
        );

    cout << "Result = "
         << result
         << endl;

    return 0;
}