// ir_generator.h
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "ast.h"     // Needed for ASTNode parameter in generate_ir
#include "ir_core.h" // For IRList, IRInstruction types

// Function declarations for IR generation
IRList* generate_ir(ASTNode *root);

// Utility functions for any IR list
void print_ir(IRList *list);
void free_ir(IRList *list); // Can free any IR list, generated or optimized

#endif // IR_GENERATOR_H