#include "compiler.h"

using namespace std;

void Compiler::compile(AST* node) {

            VariableNode* varNode =
            dynamic_cast<VariableNode*>(node);

        if (varNode != nullptr) {

            instructions.push_back(
                Instruction(
                    OP_LOAD_NAME,
                    varNode->name
                )
            );

            return;
        }

        AssignNode* assignNode =
            dynamic_cast<AssignNode*>(node);

        if (assignNode != nullptr) {

            compile(assignNode->value);

            instructions.push_back(
                Instruction(
                    OP_STORE_NAME,
                    assignNode->name
                )
            );

            return;
        }

        ProgramNode* programNode =
            dynamic_cast<ProgramNode*>(node);

        if (programNode != nullptr) {

            for (
                AST* stmt :
                programNode->statements
            ) {

                compile(stmt);
            }

            return;
        }

    NumberNode* numberNode =
        dynamic_cast<NumberNode*>(node);

    if (numberNode != nullptr) {

        instructions.push_back(
            Instruction(
                OP_LOAD_CONST,
                numberNode->value
            )
        );

        return;
    }

    CompareNode* compareNode =
    dynamic_cast<CompareNode*>(node);

        if (compareNode != nullptr) {

            compile(compareNode->left);

            compile(compareNode->right);

            if (compareNode->op == ">") {

                instructions.push_back(
                    Instruction(OP_COMPARE_GT)
                );
            }

            else if (compareNode->op == "<") {

                instructions.push_back(
                    Instruction(OP_COMPARE_LT)
                );
            }

            else if (compareNode->op == "==") {

                instructions.push_back(
                    Instruction(OP_COMPARE_EQ)
                );
            }

            return;
        }

            IfNode* ifNode =
            dynamic_cast<IfNode*>(node);

                if (ifNode != nullptr) {

                    compile(ifNode->condition);

                    int jumpIndex =
                        instructions.size();

                    instructions.push_back(
                        Instruction(
                            OP_JUMP_IF_FALSE,
                            "0"
                        )
                    );

                    for (
                        AST* stmt :
                        ifNode->ifBody
                    ) {

                        compile(stmt);
                    }

                    instructions[jumpIndex]
                        .argument =
                        to_string(
                            instructions.size()
                        );

                    return;
                }

                WhileNode* whileNode =
            dynamic_cast<WhileNode*>(node);

        if (whileNode != nullptr) {

            int loopStart =
                instructions.size();

            compile(
                whileNode->condition
            );

            int jumpIndex =
                instructions.size();

            instructions.push_back(
                Instruction(
                    OP_JUMP_IF_FALSE,
                    "0"
                )
            );

            for (
                AST* stmt :
                whileNode->body
            ) {

                compile(stmt);
            }

            instructions.push_back(
                Instruction(
                    OP_JUMP,
                    to_string(loopStart)
                )
            );

            instructions[jumpIndex]
                .argument =
                to_string(
                    instructions.size()
                );

            return;
        }

        

    BinaryOpNode* binNode =
        dynamic_cast<BinaryOpNode*>(node);

    if (binNode != nullptr) {

        compile(binNode->left);

        compile(binNode->right);

        if (binNode->op == "+") {

            instructions.push_back(
                Instruction(OP_ADD)
            );
        }

        else if (binNode->op == "-") {

            instructions.push_back(
                Instruction(OP_SUB)
            );
        }

        else if (binNode->op == "*") {

            instructions.push_back(
                Instruction(OP_MUL)
            );
        }

        else if (binNode->op == "/") {

            instructions.push_back(
                Instruction(OP_DIV)
            );
        }

        return;
    }
}

