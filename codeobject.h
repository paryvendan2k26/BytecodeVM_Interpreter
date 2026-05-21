#pragma once

#include <vector>
#include <string>

#include "instruction.h"

using namespace std;

class CodeObject {

public:

    string name;

    vector<Instruction> instructions;

    vector<int> constants;

    vector<string> names;

    vector<string> parameters;
};
