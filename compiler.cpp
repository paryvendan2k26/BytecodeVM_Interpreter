#include "compiler.h"

using namespace std;

void Compiler::compile(AST* node) {

    ProgramNode* programNode =
        dynamic_cast<ProgramNode*>(node);

    if (programNode != nullptr) {

        code.name = "<module>";

        for (AST* stmt : programNode->statements) {

            FunctionDefNode* funcNode =
                dynamic_cast<FunctionDefNode*>(stmt);

            if (funcNode != nullptr) {

                compile(stmt);
            }
        }

        for (AST* stmt : programNode->statements) {

            FunctionDefNode* funcNode =
                dynamic_cast<FunctionDefNode*>(stmt);

            if (funcNode == nullptr) {

                compile(stmt);
            }
        }

        return;
    }

    FunctionDefNode* funcNode =
        dynamic_cast<FunctionDefNode*>(node);

    if (funcNode != nullptr) {

        Compiler functionCompiler;

        functionCompiler.code.name =
            funcNode->name;

        functionCompiler.code.parameters =
            funcNode->params;

        for (
            int i = static_cast<int>(
                funcNode->params.size()
            ) - 1;
            i >= 0;
            i--
        ) {

            functionCompiler.code.instructions.push_back(
                Instruction(
                    OP_STORE_NAME,
                    funcNode->params[i]
                )
            );
        }

        for (AST* stmt : funcNode->body) {

            functionCompiler.compile(stmt);
        }

        functionCompiler.code.instructions.push_back(
            Instruction(OP_RETURN)
        );

        functions[funcNode->name] =
            functionCompiler.code;

        for (auto& entry : functionCompiler.functions) {

            functions[entry.first] =
                entry.second;
        }

        return;
    }

    FunctionCallNode* callNode =
        dynamic_cast<FunctionCallNode*>(node);

    if (callNode != nullptr) {

        for (AST* arg : callNode->args) {

            compile(arg);
        }

        code.instructions.push_back(
            Instruction(
                OP_CALL,
                callNode->name
            )
        );

        return;
    }

    ReturnNode* returnNode =
        dynamic_cast<ReturnNode*>(node);

    if (returnNode != nullptr) {

        compile(returnNode->value);

        code.instructions.push_back(
            Instruction(OP_RETURN)
        );

        return;
    }

    PrintNode* printNode =
        dynamic_cast<PrintNode*>(node);

    if (printNode != nullptr) {

        compile(printNode->value);

        code.instructions.push_back(
            Instruction(OP_PRINT)
        );

        return;
    }

    VariableNode* varNode =
        dynamic_cast<VariableNode*>(node);

    if (varNode != nullptr) {

        code.instructions.push_back(
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

        code.instructions.push_back(
            Instruction(
                OP_STORE_NAME,
                assignNode->name
            )
        );

        return;
    }

    BooleanNode* booleanNode =
        dynamic_cast<BooleanNode*>(node);

    if (booleanNode != nullptr) {

        code.constants.push_back(
            booleanNode->value ? 1 : 0
        );

        int index =
            static_cast<int>(
                code.constants.size()
            ) - 1;

        code.instructions.push_back(
            Instruction(
                OP_LOAD_CONST,
                to_string(index)
            )
        );

        return;
    }

    NumberNode* numberNode =
        dynamic_cast<NumberNode*>(node);

    if (numberNode != nullptr) {

        int value =
            stoi(numberNode->value);

        code.constants.push_back(value);

        int index =
            static_cast<int>(
                code.constants.size()
            ) - 1;

        code.instructions.push_back(
            Instruction(
                OP_LOAD_CONST,
                to_string(index)
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

            code.instructions.push_back(
                Instruction(OP_COMPARE_GT)
            );
        }

        else if (compareNode->op == ">=") {

            code.instructions.push_back(
                Instruction(OP_COMPARE_GE)
            );
        }

        else if (compareNode->op == "<") {

            code.instructions.push_back(
                Instruction(OP_COMPARE_LT)
            );
        }

        else if (compareNode->op == "<=") {

            code.instructions.push_back(
                Instruction(OP_COMPARE_LE)
            );
        }

        else if (compareNode->op == "==") {

            code.instructions.push_back(
                Instruction(OP_COMPARE_EQ)
            );
        }

        else if (compareNode->op == "!=") {

            code.instructions.push_back(
                Instruction(OP_COMPARE_NE)
            );
        }

        return;
    }

    IfNode* ifNode =
        dynamic_cast<IfNode*>(node);

    if (ifNode != nullptr) {

        compile(ifNode->condition);

        int falseJumpIndex =
            static_cast<int>(
                code.instructions.size()
            );

        code.instructions.push_back(
            Instruction(
                OP_JUMP_IF_FALSE,
                "0"
            )
        );

        for (AST* stmt : ifNode->ifBody) {

            compile(stmt);
        }

        int endJumpIndex =
            static_cast<int>(
                code.instructions.size()
            );

        code.instructions.push_back(
            Instruction(
                OP_JUMP,
                "0"
            )
        );

        code.instructions[falseJumpIndex]
            .argument =
            to_string(
                code.instructions.size()
            );

        for (AST* stmt : ifNode->elseBody) {

            compile(stmt);
        }

        code.instructions[endJumpIndex]
            .argument =
            to_string(
                code.instructions.size()
            );

        return;
    }

    WhileNode* whileNode =
        dynamic_cast<WhileNode*>(node);

    if (whileNode != nullptr) {

        int loopStart =
            static_cast<int>(
                code.instructions.size()
            );

        compile(whileNode->condition);

        int jumpIndex =
            static_cast<int>(
                code.instructions.size()
            );

        code.instructions.push_back(
            Instruction(
                OP_JUMP_IF_FALSE,
                "0"
            )
        );

        for (AST* stmt : whileNode->body) {

            compile(stmt);
        }

        code.instructions.push_back(
            Instruction(
                OP_JUMP,
                to_string(loopStart)
            )
        );

        code.instructions[jumpIndex]
            .argument =
            to_string(
                code.instructions.size()
            );

        return;
    }

    BinaryOpNode* binNode =
        dynamic_cast<BinaryOpNode*>(node);

    if (binNode != nullptr) {

        compile(binNode->left);

        compile(binNode->right);

        if (binNode->op == "+") {

            code.instructions.push_back(
                Instruction(OP_ADD)
            );
        }

        else if (binNode->op == "-") {

            code.instructions.push_back(
                Instruction(OP_SUB)
            );
        }

        else if (binNode->op == "*") {

            code.instructions.push_back(
                Instruction(OP_MUL)
            );
        }

        else if (binNode->op == "/") {

            code.instructions.push_back(
                Instruction(OP_DIV)
            );
        }

        return;
    }

    UnaryOpNode* unaryNode =
        dynamic_cast<UnaryOpNode*>(node);

    if (unaryNode != nullptr) {

        compile(unaryNode->expression);

        if (unaryNode->op == "-") {

            code.instructions.push_back(
                Instruction(OP_NEGATE)
            );
        }

        return;
    }
}
