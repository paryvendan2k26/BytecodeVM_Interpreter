#pragma once

#include "ast.h"
#include "instruction.h"
#include "codeobject.h"

#include <map>

using namespace std;

class Compiler {

public:

    CodeObject code;

    map<
        string,
        CodeObject
    > functions;

    void compile(AST* node);
};