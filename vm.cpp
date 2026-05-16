#include "vm.h"

using namespace std;

int VM::getVariable(
    string name
) {

    return frames.back()
        .locals[name];
}

void VM::setVariable(
    string name,
    int value
) {

    frames.back()
        .locals[name]
            = value;
}


int VM::run(
    vector<Instruction>& code
) {

    Frame globalFrame;

globalFrame.returnAddress = -1;

frames.push_back(globalFrame);

    int ip = 0;

    while (ip < code.size()) {

        Instruction instr =
            code[ip];

        switch (instr.opcode) {

            case OP_LOAD_CONST: {

                stack.push_back(
                    stoi(instr.argument)
                );

                break;
            }

            case OP_LOAD_NAME: {

                stack.push_back(
                    getVariable(
                        instr.argument
                    )
                );

                break;
            }

            case OP_STORE_NAME: {

                int value =
                    stack.back();

                stack.pop_back();

                setVariable(
                    instr.argument,
                    value
                );

                break;
            }

            case OP_ADD: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a + b);

                break;
            }

            case OP_SUB: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a - b);

                break;
            }

            case OP_MUL: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a * b);

                break;
            }

            case OP_DIV: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a / b);

                break;
            }

            case OP_COMPARE_GT: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a > b);

                break;
            }

            case OP_COMPARE_LT: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a < b);

                break;
            }

            case OP_COMPARE_EQ: {

                int b = stack.back();
                stack.pop_back();

                int a = stack.back();
                stack.pop_back();

                stack.push_back(a == b);

                break;
            }
            case OP_JUMP: {

                ip = stoi(instr.argument);

                continue;
            }
            case OP_JUMP_IF_FALSE: {

                int condition =
                    stack.back();

                stack.pop_back();

                if (!condition) {

                    ip = stoi(
                        instr.argument
                    );

                    continue;
                }

                break;
            }
        }

        ip++;
    }

    return stack.back();
}