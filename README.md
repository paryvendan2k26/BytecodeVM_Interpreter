# BytecodeVM Interpreter

A small C++ bytecode interpreter that demonstrates a full language pipeline:

```text
source code -> lexer -> parser/AST -> compiler -> bytecode VM
```

The VM currently runs integer-based programs with variables, branching, loops,
functions, recursion, and basic output.

## Features

- Integer literals
- Boolean literals: `true`, `false`
- Arithmetic: `+`, `-`, `*`, `/`
- Unary minus: `-value`
- Variables and assignment
- Comparisons: `>`, `>=`, `<`, `<=`, `==`, `!=`
- `if` / `else`
- `while`
- Function definitions with `func`
- Function calls and recursion
- Explicit returns with `return expr`
- Console output with `print(expr)`
- Comments with `// comment` or `# comment`

## Example

```text
// Recursive factorial
flag = true
print(flag)
print(-5)
print(5 != 4)

fact(5)

func fact(n) {
    if n == 0 {
        return 1
    }
    else {
        return n * fact(n - 1)
    }
}
```

Expected final result:

```text
Result = 120
```

## Build

```bash
g++ -std=c++17 main.cpp lexer.cpp parser.cpp compiler.cpp vm.cpp -o interpreter
```

## Run

```bash
./interpreter
```

The current entry point uses a hardcoded demo program in `main.cpp`.

## REPL

Start the interactive prompt with:

```bash
./interpreter --repl
```

Example session:

```text
>>> x = 10
=> 10
>>> print(x >= 10)
1
=> 1
>>> func double(n) {
... return n * 2
... }
=> 0
>>> double(21)
=> 42
>>> exit
```

The REPL keeps variables and function definitions available across inputs.

## Project Structure

- `lexer.*`: converts source text into tokens
- `parser.*`: converts tokens into AST nodes
- `ast.h`: AST node definitions
- `compiler.*`: converts AST nodes into bytecode instructions
- `opcode.h`: VM opcode list
- `instruction.h`: bytecode instruction representation
- `codeobject.h`: compiled code container
- `frame.h`: VM call frame representation
- `vm.*`: stack-based bytecode VM
- `main.cpp`: demo program entry point

## Good Next Improvements

- Add file input, for example `./interpreter examples/factorial.bvm`
- Add a REPL mode
- Add parser/runtime error messages with line and column numbers
- Add tests for language behavior
- Replace raw AST pointers with `std::unique_ptr`
- Add a bytecode disassembler for debugging compiled instructions
