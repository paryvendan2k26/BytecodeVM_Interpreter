#pragma once

#include <string>

#include <vector>


using namespace std;

class AST {
public:
    virtual ~AST() {}
};

class NumberNode : public AST {
public:
    string value;

    NumberNode(string val) {
        value = val;
    }
};

class BinaryOpNode : public AST {
public:
    AST* left;
    string op;
    AST* right;

    BinaryOpNode(AST* l, string o, AST* r) {
        left = l;
        op = o;
        right = r;
    }
};

class UnaryOpNode : public AST {
public:
    string op;
    AST* expression;

    UnaryOpNode(string o, AST* expr) {
        op = o;
        expression = expr;
    }
};

class VariableNode : public AST {

public:

    string name;

    VariableNode(string n) {
        name = n;
    }
};

class AssignNode : public AST {

public:

    string name;

    AST* value;

    AssignNode(string n, AST* v) {

        name = n;

        value = v;
    }
};


class ProgramNode : public AST {

public:

    vector<AST*> statements;

    ProgramNode(vector<AST*> stmts) {

        statements = stmts;
    }
};

class IfNode : public AST {

public:

    AST* condition;

    vector<AST*> ifBody;

    vector<AST*> elseBody;

    IfNode(
        AST* cond,
        vector<AST*> ifStmts,
        vector<AST*> elseStmts
    ) {

        condition = cond;

        ifBody = ifStmts;

        elseBody = elseStmts;
    }
};


class CompareNode : public AST {

public:

    AST* left;

    string op;

    AST* right;

    CompareNode(
        AST* l,
        string o,
        AST* r
    ) {

        left = l;

        op = o;

        right = r;
    }
};

class WhileNode : public AST {

public:

    AST* condition;

    vector<AST*> body;

    WhileNode(
        AST* cond,
        vector<AST*> stmts
    ) {

        condition = cond;

        body = stmts;
    }
};

class BooleanNode : public AST {

public:

    bool value;

    BooleanNode(bool v) {

        value = v;
    }
};

class FunctionDefNode : public AST {

public:

    string name;

    vector<string> params;

    vector<AST*> body;

    FunctionDefNode(
        string n,
        vector<string> p,
        vector<AST*> b
    ) {

        name = n;

        params = p;

        body = b;
    }
};

class FunctionCallNode : public AST {

public:

    string name;

    vector<AST*> args;

    FunctionCallNode(
        string n,
        vector<AST*> a
    ) {

        name = n;

        args = a;
    }
};

class ReturnNode : public AST {

public:

    AST* value;

    ReturnNode(AST* v) {

        value = v;
    }
};

class PrintNode : public AST {

public:

    AST* value;

    PrintNode(AST* v) {

        value = v;
    }
};
