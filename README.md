# BytecodeVM Interpreter

A custom stack-based bytecode virtual machine and compiler written in modern C++.

This project implements a complete language execution pipeline:

```text
Source Code → Lexer → Parser → AST → Bytecode Compiler → VM Execution
```

Unlike traditional tree-walk interpreters, the AST is compiled into custom bytecode instructions executed by a dedicated virtual machine. The architecture is inspired by CPython, Lua VM, and JVM-style execution models.

---

# Core Components

### Lexer
Converts raw source code into tokens such as identifiers, literals, operators, and keywords.

### Recursive Descent Parser
Builds an Abstract Syntax Tree (AST) with proper operator precedence and expression handling.

### Bytecode Compiler
Transforms AST nodes into low-level VM instructions.

Example:

```text
PUSH 10
PUSH 20
ADD
STORE x
```

### Stack-Based Virtual Machine
Executes bytecode using a runtime operand stack, instruction pointer, and call-frame system.

Supports:

- Arithmetic Expressions
- Variables & Assignment
- Comparisons
- Conditional Execution
- While Loops
- Functions
- Recursion
- Local & Global Scope Management

---

# VM Architecture

The VM uses:

- Stack-based execution
- Custom instruction set architecture (ISA)
- Call frames for function execution
- Runtime scope isolation
- Bytecode dispatch loop

Example instructions:

```text
PUSH
LOAD
STORE
ADD
SUB
MUL
DIV
JMP
CALL
RETURN
```

---

# Build

```bash
g++ main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp -o vm
```

Run:

```bash
./vm
```

---

# Technical Highlights

- Recursive Descent Parsing
- AST Generation
- Bytecode Compilation
- Stack Machine Design
- Runtime Frame Management
- Instruction Dispatching
- Scope Resolution
- Function Execution Model

---

# Future Work

- Garbage Collection
- Heap Objects
- Closures
- Classes & Objects
- Bytecode Optimizations
- REPL & Debugger

---

# Inspiration

Inspired by:

- CPython VM
- Lua VM
- JVM
- Crafting Interpreters

- # References & Inspiration

This project was heavily inspired by:

- "A Python Interpreter Written in Python" from *The Architecture of Open Source Applications (AOSA)*
- Crafting Interpreters by Robert Nystrom
- CPython and Lua VM internals

AOSA Reference:
https://aosabook.org/en/500L/a-python-interpreter-written-in-python.html

The project extends those concepts further by implementing a custom bytecode compiler and stack-based virtual machine in C++.
