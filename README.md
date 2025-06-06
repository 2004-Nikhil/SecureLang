# Safeguard Programming Language Compiler

A secure, statically-typed programming language compiler with built-in security analysis and bytecode generation capabilities.

## Overview

Safeguard is a programming language designed with security in mind, featuring:
- Static type checking with semantic analysis
- Taint analysis for tracking potentially unsafe data
- Range analysis for detecting integer overflows
- Array bounds checking
- Intermediate representation (IR) optimization
- Bytecode generation for virtual machine execution

## Features

### Language Features
- **Basic Types**: `number` (32-bit signed integers), `text` (strings with max length), `array` (fixed-size number arrays)
- **Control Flow**: `if/else` statements, `while` loops
- **Functions**: User-defined functions with parameters and return values
- **Built-in Functions**: 
  - `print_number(n)` - Print a number
  - `print_text(s)` - Print text
  - `read_number()` - Read number from input (marked as tainted)
  - `size_of(array)` - Get array size

### Security Features
- **Taint Analysis**: Tracks data from external sources (like `read_number()`)
- **Range Analysis**: Tracks value ranges to detect potential overflows
- **Bounds Checking**: Warns about potential array access violations
- **Integer Overflow Detection**: Identifies operations that may overflow

### Compiler Pipeline
1. **Lexical Analysis**: Tokenizes source code
2. **Parsing**: Builds Abstract Syntax Tree (AST)
3. **Semantic Analysis**: Type checking, symbol table management, security analysis
4. **IR Generation**: Creates three-address code intermediate representation
5. **Optimization**: Constant folding, propagation, dead code elimination
6. **Bytecode Generation**: Produces bytecode for virtual machine execution

## Project Structure

```
├── ast.h/.c                 # Abstract Syntax Tree definitions and utilities
├── symbol_table.h/.c        # Symbol table and scope management
├── semantic_analyzer.h/.c   # Type checking and security analysis
├── ir_core.h                # IR instruction definitions
├── ir_generator.h/.c        # IR generation from AST
├── ir_optimizer.h/.c        # IR optimization passes
├── bytecode_opcodes.h       # Bytecode instruction definitions
├── bytecode_generator.h/.c  # Bytecode generation from IR
└── test_safeguard.sgl       # Example Safeguard source file
```

## Building the Project

### Prerequisites
- GCC or compatible C compiler
- Make (optional, for build automation)

### Manual Compilation
```bash
# Compile all source files
gcc -o safeguard_compiler safeguard.tab.c lex.yy.c ast.c symbol_table.c semantic_analyzer.c ir_generator.c ir_optimizer.c bytecode_generator.c -lfl
```

## Usage

### Basic Compilation
```bash
# Analyze a Safeguard source file
./safeguard_compiler test_safeguard.sgl
```

### Example Safeguard Program
```safeguard
// Variable declarations with types
number x = 10;
text[50] message = "Hello, World!";
array[5] numbers;

// Function definition
number add(number a, number b) {
    return a + b;
}

// Main program logic
if (x > 5) {
    print_text(message);
    numbers[0] = add(x, 5);
    print_number(numbers[0]);
}

// Input handling (automatically marked as tainted)
number user_input = read_number();
if (user_input >= 0 && user_input < size_of(numbers)) {
    numbers[user_input] = 42;  // Safe array access
}
```

## Language Syntax

### Types
- `number` - 32-bit signed integer
- `text[N]` - String with maximum length N
- `array[N]` - Array of N numbers

### Variable Declaration
```safeguard
number variable_name = initial_value;
text[max_length] string_var = "initial string";
array[size] array_var;
```

### Control Structures
```safeguard
// If statement
if (condition) {
    // statements
} else {
    // statements
}

// While loop
while (condition) {
    // statements
}
```

### Functions
```safeguard
return_type function_name(parameter_type param_name, ...) {
    // function body
    return value;  // if return_type is not void
}
```

## Architecture Details

### Symbol Table
- Hierarchical scope management
- Type information storage
- Value range tracking
- Taint status tracking

### IR Optimization
- **Constant Folding**: Evaluates constant expressions at compile time
- **Constant Propagation**: Replaces variables with known constant values
- **Dead Code Elimination**: Removes unused temporary variables and computations

### Bytecode Format
Stack-based virtual machine bytecode with operations for:
- Arithmetic and comparison operations
- Memory management (load/store)
- Control flow (jumps, conditional branches)
- Function calls and returns
- I/O operations

## Development Status

This is a educational/research compiler implementation demonstrating:
- Security-focused language design
- Multi-pass compilation architecture
- Static analysis techniques
- Bytecode generation for virtual machines

## Contributing

This project serves as a reference implementation for secure language design and compiler construction techniques.
