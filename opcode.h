#pragma once

enum OpCode {

    OP_LOAD_CONST,

    OP_LOAD_NAME,

    OP_STORE_NAME,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_COMPARE_GT,
    OP_COMPARE_GE,
    OP_COMPARE_LT,
    OP_COMPARE_LE,
    OP_COMPARE_EQ,
    OP_COMPARE_NE,

    OP_NEGATE,
    OP_PRINT,

    OP_JUMP_IF_FALSE,
    OP_JUMP,

    OP_CALL,

    OP_RETURN
};
