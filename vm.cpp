#include "vm.h"

#include <iostream>
#include <stdexcept>

using namespace std;

int VM::getVariable(string name) {

    Frame& frame =
        frames.back();

    if (
        frame.locals.find(name) !=
        frame.locals.end()
    ) {

        return frame.locals[name];
    }

    if (
        frame.globals != nullptr &&
        frame.globals->find(name) !=
        frame.globals->end()
    ) {

        return (*frame.globals)[name];
    }

    return 0;
}

void VM::setVariable(
    string name,
    int value
) {

    Frame& frame =
        frames.back();

    if (
        frame.globals == nullptr ||
        frame.code == nullptr ||
        frame.code->name != "<module>"
    ) {

        frame.locals[name] = value;

        return;
    }

    (*frame.globals)[name] = value;
}

int VM::run(
    CodeObject& code,
    bool resetGlobals
) {

    frames.clear();

    stack.clear();

    if (resetGlobals) {

        globals.clear();
    }

    frames.push_back(
        Frame(
            &code,
            &globals
        )
    );

    int lastValue = 0;

    while (!frames.empty()) {

        Frame& frame =
            frames.back();

        if (
            frame.code == nullptr ||
            frame.ip >=
            static_cast<int>(
                frame.code->instructions.size()
            )
        ) {

            if (!frame.stack.empty()) {

                lastValue =
                    frame.stack.back();
            }

            frames.pop_back();

            if (
                !frames.empty()
            ) {

                frames.back()
                    .stack
                    .push_back(lastValue);
            }

            continue;
        }

        Instruction instr =
            frame.code
                ->instructions[
                    frame.ip
                ];

        frame.ip++;

        switch (instr.opcode) {

            case OP_LOAD_CONST: {

                int value =
                    frame.code
                        ->constants[
                            stoi(instr.argument)
                        ];

                frame.stack.push_back(value);

                break;
            }

            case OP_LOAD_NAME: {

                frame.stack.push_back(
                    getVariable(
                        instr.argument
                    )
                );

                break;
            }

            case OP_STORE_NAME: {

                int value =
                    frame.stack.back();

                frame.stack.pop_back();

                setVariable(
                    instr.argument,
                    value
                );

                break;
            }

            case OP_ADD: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a + b);

                break;
            }

            case OP_SUB: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a - b);

                break;
            }

            case OP_MUL: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a * b);

                break;
            }

            case OP_DIV: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a / b);

                break;
            }

            case OP_COMPARE_GT: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a > b);

                break;
            }

            case OP_COMPARE_GE: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a >= b);

                break;
            }

            case OP_COMPARE_LT: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a < b);

                break;
            }

            case OP_COMPARE_LE: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a <= b);

                break;
            }

            case OP_COMPARE_EQ: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a == b);

                break;
            }

            case OP_COMPARE_NE: {

                int b = frame.stack.back();
                frame.stack.pop_back();

                int a = frame.stack.back();
                frame.stack.pop_back();

                frame.stack.push_back(a != b);

                break;
            }

            case OP_NEGATE: {

                int value =
                    frame.stack.back();

                frame.stack.pop_back();

                frame.stack.push_back(-value);

                break;
            }

            case OP_PRINT: {

                int value =
                    frame.stack.back();

                frame.stack.pop_back();

                cout << value << endl;

                frame.stack.push_back(value);

                break;
            }

            case OP_JUMP: {

                frame.ip =
                    stoi(instr.argument);

                break;
            }

            case OP_JUMP_IF_FALSE: {

                int condition =
                    frame.stack.back();

                frame.stack.pop_back();

                if (!condition) {

                    frame.ip =
                        stoi(instr.argument);
                }

                break;
            }

            case OP_CALL: {

                if (
                    functions.find(instr.argument) ==
                    functions.end()
                ) {

                    throw runtime_error(
                        "Undefined function: " +
                        instr.argument
                    );
                }

                CodeObject& functionCode =
                    functions[instr.argument];

                Frame newFrame(
                    &functionCode,
                    &globals
                );

                vector<int> args;

                for (
                    int i = 0;
                    i < static_cast<int>(
                        functionCode.parameters.size()
                    );
                    i++
                ) {

                    args.push_back(
                        frame.stack.back()
                    );

                    frame.stack.pop_back();
                }

                for (
                    int i = static_cast<int>(
                        args.size()
                    ) - 1;
                    i >= 0;
                    i--
                ) {

                    newFrame.stack.push_back(
                        args[i]
                    );
                }

                frames.push_back(newFrame);

                break;
            }

            case OP_RETURN: {

                if (!frame.stack.empty()) {

                    lastValue =
                        frame.stack.back();

                    frame.stack.pop_back();
                }

                frames.pop_back();

                if (frames.empty()) {

                    return lastValue;
                }

                frames.back()
                    .stack
                    .push_back(lastValue);

                break;
            }
        }
    }

    stack.push_back(lastValue);

    return lastValue;
}
