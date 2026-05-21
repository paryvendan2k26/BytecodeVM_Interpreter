#include <iostream>

#include "lexer.h"
#include "parser.h"

#include "compiler.h"
#include "vm.h"

using namespace std;

int main() {

string input = 

"// Recursive factorial demo with explicit return and print.\n"
"flag = true\n"
"print(flag)\n"
"print(-5)\n"
"print(5 != 4)\n"
"print(5 >= 5)\n"
"print(3 <= 4)\n"
"fact(5)\n"

"func fact(n) {\n"

"if n == 0 {\n"
"return 1\n"
"}\n"

"else {\n"
"return n * fact(n - 1)\n"
"}\n"

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

    int result = vm.run(
        compiler.code
    );

    cout << "Result = "
         << result
         << endl;

    return 0;
}
