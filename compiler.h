#pragma once

#include "ast.h"
#include "instruction.h"

#include <vector>
#include <map>

using namespace std;

class Compiler {

public:

    vector<Instruction>
        instructions;

    map<string,int>
        functions;

    void compile(AST* node);
};