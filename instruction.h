#pragma once

#include <string>

#include "opcode.h"

using namespace std;

class Instruction {

public:

    OpCode opcode;

    string argument;

    Instruction(
        OpCode op,
        string arg = ""
    ) {

        opcode = op;

        argument = arg;
    }
};