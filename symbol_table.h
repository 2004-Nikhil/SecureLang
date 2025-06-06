#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h" // For ASTNodeType, ASTNode, ASTNodeList

// Define what kind of symbol it is
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    // SYMBOL_TYPE // If you add user-defined types
} SymbolKind;

// For numeric range analysis
typedef struct {
    long long min; // Use long long to detect overflow during range calculation
    long long max;
    int is_known; // Flag to indicate if the range is actually determined
    int is_tainted; // For taint tracking
} ValueRange;

// Defined 32-bit integer limits
#define SAFEGUARD_INT_MIN (-2147483647LL - 1LL)
#define SAFEGUARD_INT_MAX 2147483647LL

// Entry in the symbol table
typedef struct SymbolTableEntry {
    char *name;             // Symbol name (identifier)
    SymbolKind kind;        // Variable, Function, etc.
    ASTNode *type_node;     // Pointer to AST_TYPE_SPECIFIER node defining its type
                            // For functions, this would be the return type
    int scope_level;        // The lexical level of the scope it's defined in
    int line_declared;      // Line number where declared

    // For functions:
    ASTNodeList *parameters; // Linked list of AST_VAR_DECL nodes for parameters
    int is_defined;         // For functions, 0 if just declared (prototype), 1 if body defined

    // For vulnerability analysis (primarily for variables):
    int is_initialized;     // Has it been assigned a value?
    ValueRange value_range; // Estimated range of values

    struct SymbolTableEntry *next_in_bucket; // For linked list in hash table bucket
} SymbolTableEntry;


// Scope structure
#define HASH_TABLE_SIZE 256 // Power of 2 often good for modulo hashing

typedef struct Scope {
    SymbolTableEntry *hash_table[HASH_TABLE_SIZE];
    struct Scope *parent;   // Pointer to enclosing scope
    int level;              // Scope depth (0 for global, 1 for first level, etc.)
    // You might add other scope-specific info here if needed
} Scope;


// --- Global Symbol Table Management ---
void symbol_table_init();    // Initializes the symbol table system (creates global scope)
void symbol_table_destroy(); // Destroys all scopes and entries

// --- Scope Management API ---
void symbol_table_enter_scope();
void symbol_table_exit_scope();
Scope* symbol_table_current_scope();
int symbol_table_current_level();

// --- Symbol Manipulation API ---
// Inserts a symbol into the *current* scope.
// Returns a pointer to the new entry, or NULL if re-declaration in current scope or other error.
SymbolTableEntry* symbol_table_insert(const char *name, SymbolKind kind, ASTNode *type_node, int line_declared);

// Looks up a symbol starting from the current scope and going outwards to parent scopes.
// Returns pointer to entry if found, NULL otherwise.
SymbolTableEntry* symbol_table_lookup(const char *name);

// Looks up a symbol only in the *current* scope.
// Returns pointer to entry if found, NULL otherwise.
SymbolTableEntry* symbol_table_lookup_in_current_scope(const char *name);

// --- ValueRange Utility Functions ---
ValueRange range_unknown();
ValueRange range_const(long long val);
ValueRange range_add(ValueRange r1, ValueRange r2);
ValueRange range_subtract(ValueRange r1, ValueRange r2);
ValueRange range_multiply(ValueRange r1, ValueRange r2);
ValueRange range_divide(ValueRange r1, ValueRange r2);   // Watch for division by zero
ValueRange range_modulo(ValueRange r1, ValueRange r2);  // Watch for division by zero
ValueRange range_unary_minus(ValueRange r);
void print_range(ValueRange r); // For debugging


#endif // SYMBOL_TABLE_H
