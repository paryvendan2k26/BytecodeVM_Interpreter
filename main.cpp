#include <iostream>
#include <stdexcept>
#include <string>

#include "lexer.h"
#include "parser.h"

#include "compiler.h"
#include "vm.h"

using namespace std;

int executeSource(
    const string& input,
    VM& vm,
    bool resetGlobals
) {

    Lexer lexer(input);

    vector<Token> tokens =
        lexer.tokenize();

    Parser parser(tokens);

    AST* tree =
        parser.parse();

    Compiler compiler;

    compiler.compile(tree);

    for (auto& entry : compiler.functions) {

        vm.functions[entry.first] =
            entry.second;
    }

    return vm.run(
        compiler.code,
        resetGlobals
    );
}

int braceBalance(const string& input) {

    int balance = 0;

    for (char ch : input) {

        if (ch == '{') {

            balance++;
        }

        else if (ch == '}') {

            balance--;
        }
    }

    return balance;
}

void runRepl() {

    VM vm;
    string line;
    string source;

    cout << "BytecodeVM REPL. Type exit or quit to stop." << endl;

    while (true) {

        cout << (source.empty() ? ">>> " : "... ");

        if (!getline(cin, line)) {

            break;
        }

        if (
            source.empty() &&
            (line == "exit" || line == "quit")
        ) {

            break;
        }

        source += line + "\n";

        if (braceBalance(source) > 0) {

            continue;
        }

        try {

            int result =
                executeSource(
                    source,
                    vm,
                    false
                );

            cout << "=> "
                 << result
                 << endl;
        }

        catch (const exception& error) {

            cout << "Error: "
                 << error.what()
                 << endl;
        }

        source.clear();
    }
}

int main(
    int argc,
    char* argv[]
) {

    if (
        argc > 1 &&
        string(argv[1]) != "--repl"
    ) {

        cout << "Usage: "
             << argv[0]
             << " [--repl]"
             << endl;

        return 1;
    }

    runRepl();

    return 0;
}
