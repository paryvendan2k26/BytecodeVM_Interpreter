#pragma once

#include <vector>
#include <map>
#include <string>

#include "instruction.h"
#include "frame.h"
#include "codeobject.h"

using namespace std;

class VM {

public:

    vector<int> stack;

    vector<Frame> frames;

    map<string, int> globals;

    map<
        string,
        CodeObject
    > functions;

    int getVariable(string name);

    void setVariable(string name, int value);

    int run(
        CodeObject& code,
        bool resetGlobals = true
    );
};
