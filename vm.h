#pragma once

#include <vector>
#include <map>

#include "instruction.h"

using namespace std;

class VM {

public:

    vector<int> stack;

    map<string, int> variables;

    int run(
        vector<Instruction>& code
    );
};