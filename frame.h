#pragma once

#include <map>
#include <string>
#include <vector>

#include "codeobject.h"

using namespace std;

class Frame {

public:

    CodeObject* code;

    int ip;

    vector<int> stack;

    map<string, int> locals;

    map<string, int>* globals;

    Frame(
        CodeObject* codeObject = nullptr,
        map<string, int>* globalScope = nullptr
    ) {

        code = codeObject;

        ip = 0;

        globals = globalScope;
    }
};
