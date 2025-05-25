#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include "symbol_table.h" // For symbol table types

// Entry point for semantic analysis
// Returns the number of semantic errors and vulnerability warnings found.
// Modifies the AST by annotating nodes (e.g., with types, symbol table refs).
int analyze_semantics(ASTNode *program_node);

// You might have static helper functions declared here if needed by multiple .c files,
// but most traversal functions will be static in semantic_analyzer.c

#endif // SEMANTIC_ANALYZER_H
