#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"

using namespace std;

int main() {

string input =
"x = 5 x == 5"; 
Lexer lexer(input);

    vector<Token> tokens =
        lexer.tokenize();

    Parser parser(tokens);

    AST* tree =
        parser.parse();

    Compiler compiler;

    compiler.compile(tree);

    VM vm;

    int result =
        vm.run(
            compiler.instructions
        );

    cout
        << "Result = "
        << result
        << endl;

    return 0;
}