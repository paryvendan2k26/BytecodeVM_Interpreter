#include "compiler.h"

using namespace std;

void Compiler::compile(AST* node) {

    FunctionDefNode* funcNode =
    dynamic_cast<FunctionDefNode*>(node);

        if (funcNode != nullptr) {

            functions[
                funcNode->name
            ] = instructions.size();

            for (
                string param :
                funcNode->params
            ) {

                instructions.push_back(
                    Instruction(
                        OP_STORE_NAME,
                        param
                    )
                );
            }

            for (
                AST* stmt :
                funcNode->body
            ) {

                compile(stmt);
            }

            instructions.push_back(
                Instruction(OP_RETURN)
            );

            return;
        }

        FunctionCallNode* callNode =
    dynamic_cast<FunctionCallNode*>(node);

            if (callNode != nullptr) {

                for (
                    AST* arg :
                    callNode->args
                ) {

                    compile(arg);
                }

                instructions.push_back(
                    Instruction(
                        OP_CALL,
                        callNode->name
                    )
                );

                return;
            }

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

                int jumpIndex =
                    instructions.size();

                instructions.push_back(
                    Instruction(
                        OP_JUMP,
                        "0"
                    )
                );

                vector<AST*> functionsList;
                vector<AST*> mainStatements;

                for (
                    AST* stmt :
                    programNode->statements
                ) {

                    FunctionDefNode* funcNode =

                        dynamic_cast<
                            FunctionDefNode*
                        >(stmt);

                    if (funcNode != nullptr) {

                        functionsList.push_back(
                            stmt
                        );
                    }

                    else {

                        mainStatements.push_back(
                            stmt
                        );
                    }
                }

                for (
                    AST* stmt :
                    functionsList
                ) {

                    compile(stmt);
                }

                instructions[jumpIndex]
                    .argument =
                    to_string(
                        instructions.size()
                    );

                for (
                    AST* stmt :
                    mainStatements
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

                int falseJumpIndex =
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

                int endJumpIndex =
                    instructions.size();

                instructions.push_back(
                    Instruction(
                        OP_JUMP,
                        "0"
                    )
                );

                instructions[falseJumpIndex]
                    .argument =
                    to_string(
                        instructions.size()
                    );

                for (
                    AST* stmt :
                    ifNode->elseBody
                ) {

                    compile(stmt);
                }

                instructions[endJumpIndex]
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

