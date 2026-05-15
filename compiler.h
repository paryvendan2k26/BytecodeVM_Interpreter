#pragma once

#include <vector>

#include "instruction.h"
#include "ast.h"

using namespace std;

class Compiler {

public:

    vector<Instruction> instructions;

    void compile(AST* node);
};