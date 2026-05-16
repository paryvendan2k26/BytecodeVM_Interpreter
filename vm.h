#pragma once

#include <vector>
#include <map>

#include "instruction.h"

using namespace std;

class Frame {

public:

    map<string,int> locals;

    int returnAddress;
};

class VM {

public:

    vector<int> stack;

    vector<Frame> frames;

    map<string,int> functions;


    int getVariable(string name);

        void setVariable(
            string name,
            int value
        );




    int run(
        vector<Instruction>& code
    );
};