#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h" // Though not strictly needed here if types are opaque

// --- Global State for Symbol Table ---
static Scope *current_scope_ptr = NULL;
static int next_scope_level = 0;

// --- Hash Function ---
// Simple djb2 hash
static unsigned int hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % HASH_TABLE_SIZE;
}

// --- ValueRange Utility Implementations ---
ValueRange range_unknown() {
    ValueRange r;
    r.is_known = 0;
    r.is_tainted = 0; // Default to untainted
    r.min = 0; // Or some sentinel
    r.max = 0; // Or some sentinel
    return r;
}

ValueRange range_const(long long val) {
    ValueRange r;
    r.min = val;
    r.max = val;
    r.is_known = 1;
    r.is_tainted = 0;
    return r;
}

// Clamp value to 32-bit signed int range for range arithmetic results
static long long clamp_to_int32(long long val) {
    if (val < SAFEGUARD_INT_MIN) return SAFEGUARD_INT_MIN;
    if (val > SAFEGUARD_INT_MAX) return SAFEGUARD_INT_MAX;
    return val;
}

ValueRange range_add(ValueRange r1, ValueRange r2) {
    if (!r1.is_known || !r2.is_known) return range_unknown();
    ValueRange res;
    res.min = clamp_to_int32(r1.min + r2.min);
    res.max = clamp_to_int32(r1.max + r2.max);
    // Check for overflow during intermediate calculation if r1.min + r2.min itself overflows long long
    // This simplified version assumes long long is wide enough for intermediate sums.
    res.is_known = 1;
    res.is_tainted = r1.is_tainted || r2.is_tainted;
    return res;
}

ValueRange range_subtract(ValueRange r1, ValueRange r2) {
    if (!r1.is_known || !r2.is_known) return range_unknown();
    ValueRange res;
    res.min = clamp_to_int32(r1.min - r2.max); // min - max
    res.max = clamp_to_int32(r1.max - r2.min); // max - min
    res.is_known = 1;
    res.is_tainted = r1.is_tainted || r2.is_tainted;
    return res;
}

ValueRange range_multiply(ValueRange r1, ValueRange r2) {
    if (!r1.is_known || !r2.is_known) return range_unknown();
    ValueRange res = range_unknown();
    res.is_known = 1;
    res.is_tainted = r1.is_tainted || r2.is_tainted;

    // Product of intervals is more complex: need to check all 4 boundary products
    long long p1 = r1.min * r2.min;
    long long p2 = r1.min * r2.max;
    long long p3 = r1.max * r2.min;
    long long p4 = r1.max * r2.max;

    res.min = p1;
    if (p2 < res.min) res.min = p2;
    if (p3 < res.min) res.min = p3;
    if (p4 < res.min) res.min = p4;

    res.max = p1;
    if (p2 > res.max) res.max = p2;
    if (p3 > res.max) res.max = p3;
    if (p4 > res.max) res.max = p4;
    
    res.min = clamp_to_int32(res.min);
    res.max = clamp_to_int32(res.max);
    return res;
}

ValueRange range_unary_minus(ValueRange r) {
    if (!r.is_known) return range_unknown();
    ValueRange res;
    res.min = clamp_to_int32(-r.max);
    res.max = clamp_to_int32(-r.min);
    // Special case for -INT_MIN not overflowing if long long is wider
    if (r.max == SAFEGUARD_INT_MIN) res.min = SAFEGUARD_INT_MIN; // -(-MAX-1) can't be +MAX+1 if MAX is 2^31-1
    if (r.min == SAFEGUARD_INT_MIN) res.max = SAFEGUARD_INT_MAX; // Smallest value * -1 = largest positive (if it fits)
                                                              // but -INT_MIN for 2s complement usually overflows to INT_MIN
                                                              // Let's assume -INT32_MIN overflows to INT32_MIN for simplicity in this check.
                                                              // A true check would note the overflow.
    res.is_known = 1;
    res.is_tainted = r.is_tainted;
    return res;
}

ValueRange range_divide(ValueRange r1, ValueRange r2) {
    // Division is complex, especially with 0. For now, return unknown.
    // A more robust implementation would handle different cases.
    // If r2 contains 0, the range is effectively [-inf, +inf] or error.
    if (!r1.is_known || !r2.is_known) return range_unknown();
    if ((r2.min <= 0 && r2.max >= 0)) { // Potential division by zero
         // semantic_warning("Potential division by zero in range analysis.", 0 /* no line here */);
         // For now, just make it unknown if zero is in divisor's range
         ValueRange res = range_unknown();
         res.is_tainted = r1.is_tainted || r2.is_tainted;
         return res;
    }
    // Simplistic: just take boundaries, not fully correct for all interval divisions
    long long v[4];
    v[0] = r1.min / r2.min;
    v[1] = r1.min / r2.max;
    v[2] = r1.max / r2.min;
    v[3] = r1.max / r2.max;

    ValueRange res = range_unknown();
    res.is_known = 1;
    res.is_tainted = r1.is_tainted || r2.is_tainted;
    res.min = res.max = v[0];
    for(int i=1; i<4; ++i) {
        if (v[i] < res.min) res.min = v[i];
        if (v[i] > res.max) res.max = v[i];
    }
    res.min = clamp_to_int32(res.min);
    res.max = clamp_to_int32(res.max);
    return res;
}

ValueRange range_modulo(ValueRange r1, ValueRange r2) {
    // Modulo range is also complex. Max possible result is r2.max-1 (if r2.max > 0). Min is 0 if r1 is positive.
    // If r2 contains 0, similar issue to division.
    if (!r1.is_known || !r2.is_known) return range_unknown();
    if ((r2.min <= 0 && r2.max >= 0)) {
         ValueRange res = range_unknown();
         res.is_tainted = r1.is_tainted || r2.is_tainted;
         return res;
    }
    // Simplification: If r2 is a positive constant, result is [0, r2.val-1] or [- (r2.val-1), r2.val-1]
    // For now, return unknown for simplicity of this example code.
    ValueRange res = range_unknown();
    res.is_tainted = r1.is_tainted || r2.is_tainted;
    // A better approximation: if r2.min > 0, then result is in [0, r2.max - 1]
    // if r2.max < 0, then result is in [r2.min + 1, 0] (for negative divisors)
    // This needs more care.
    return res;
}


void print_range(ValueRange r) {
    if (r.is_known) {
        printf("[%lld, %lld]", r.min, r.max);
    } else {
        printf("[unknown]");
    }
    if (r.is_tainted) {
        printf(" (tainted)");
    }
}


// --- Symbol Table API Implementations ---

static ASTNode* type_node_number = NULL;
static ASTNode* type_node_text = NULL;
static ASTNode* type_node_void = NULL; // Representing void return type
void symbol_table_init() {
    if (current_scope_ptr != NULL) {
        // This shouldn't happen if init is called only once
        fprintf(stderr, "Symbol table already initialized.\n");
        return;
    }
    current_scope_ptr = (Scope *)malloc(sizeof(Scope));
    if (!current_scope_ptr) {
        perror("Failed to allocate global scope");
        exit(EXIT_FAILURE);
    }
    current_scope_ptr->parent = NULL;
    current_scope_ptr->level = next_scope_level++; // Global scope is level 0
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        current_scope_ptr->hash_table[i] = NULL;
    }
    printf("Symbol Table: Global scope (level 0) created.\n");
    
    if (!type_node_number) type_node_number = create_type_spec_node("number", 0, 0);
    if (!type_node_text) type_node_text = create_type_spec_node("text", 0, 0);
    
    SymbolTableEntry* entry;

    // read_number() returns number
    entry = symbol_table_insert("read_number", SYMBOL_FUNCTION, type_node_number, 0);
    if (entry) {
        entry->is_defined = 1; // Built-ins are always "defined"
        // entry->parameters = NULL; // No parameters
    }

    // print_number(number) returns void
    entry = symbol_table_insert("print_number", SYMBOL_FUNCTION, NULL /* void return */, 0);
    if (entry) {
        entry->is_defined = 1;
        // TODO: Define parameter list structure and populate it here
        // For now, semantic analyzer will check args specifically for built-ins.
        // ASTNode* param_type_num = create_type_spec_node("number", 0, 0);
        // entry->parameters = create_node_list(create_var_decl_node("value", param_type_num, NULL, 0));
    }

    // print_text(text) returns void
    entry = symbol_table_insert("print_text", SYMBOL_FUNCTION, NULL /* void return */, 0);
    if (entry) {
        entry->is_defined = 1;
        // ASTNode* param_type_text = create_type_spec_node("text", 0, 0); // Assuming text type needs a size for params too if not literal
        // entry->parameters = create_node_list(create_var_decl_node("message", param_type_text, NULL, 0));
    }

    // size_of(array_identifier) returns number - this one is special
    // It's more like an operator. The semantic analyzer handles its argument specially.
    // We can still add it as a function symbol for lookup consistency.
    entry = symbol_table_insert("size_of", SYMBOL_FUNCTION, type_node_number, 0);
    if (entry) entry->is_defined = 1;

}



// Helper to free a single scope's entries
static void free_scope_entries(Scope* scope) {
    if (!scope) return;
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        SymbolTableEntry *entry = scope->hash_table[i];
        while (entry) {
            SymbolTableEntry *temp = entry;
            entry = entry->next_in_bucket;
            free(temp->name); // Name was strdup'd
            // ASTNode* type_node is owned by the AST, not freed here usually
            // ASTNodeList* parameters also owned by AST
            free(temp);
        }
        scope->hash_table[i] = NULL; // Defensive
    }
}

void symbol_table_destroy() {
    while (current_scope_ptr) {
        Scope *parent_scope = current_scope_ptr->parent;
        free_scope_entries(current_scope_ptr);
        free(current_scope_ptr);
        current_scope_ptr = parent_scope;
    }
    next_scope_level = 0; // Reset for potential re-initialization (e.g. in tests)
    if (type_node_number) { free_ast(type_node_number); type_node_number = NULL; }
    if (type_node_text)   { free_ast(type_node_text);   type_node_text = NULL; }
    printf("Symbol Table: All scopes destroyed.\n");
}


void symbol_table_enter_scope() {
    Scope *new_scope = (Scope *)malloc(sizeof(Scope));
    if (!new_scope) {
        perror("Failed to allocate new scope");
        exit(EXIT_FAILURE); // Or handle error more gracefully
    }
    new_scope->parent = current_scope_ptr;
    new_scope->level = next_scope_level++;
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
        new_scope->hash_table[i] = NULL;
    }
    current_scope_ptr = new_scope;
    // printf("Symbol Table: Entered scope level %d.\n", current_scope_ptr->level);
}

void symbol_table_exit_scope() {
    if (!current_scope_ptr) {
        fprintf(stderr, "Symbol Table Error: Attempt to exit NULL scope (already global?).\n");
        return;
    }
    if (current_scope_ptr->parent == NULL) { // Cannot exit global scope this way
        fprintf(stderr, "Symbol Table Error: Attempt to exit global scope.\n");
        return;
    }
    Scope *parent_scope = current_scope_ptr->parent;
    // printf("Symbol Table: Exiting scope level %d, returning to %d.\n", current_scope_ptr->level, parent_scope->level);
    free_scope_entries(current_scope_ptr);
    free(current_scope_ptr);
    current_scope_ptr = parent_scope;
    next_scope_level--; // Decrement available scope level counter
}

Scope* symbol_table_current_scope() {
    return current_scope_ptr;
}
int symbol_table_current_level() {
    if (current_scope_ptr) {
        return current_scope_ptr->level;
    }
    return -1; // Should not happen if initialized
}


SymbolTableEntry* symbol_table_insert(const char *name, SymbolKind kind, ASTNode *type_node, int line_declared) {
    if (!current_scope_ptr) {
        fprintf(stderr, "Symbol Table Error: No current scope to insert into (not initialized?).\n");
        return NULL;
    }

    // Check for re-declaration in the *current* scope
    if (symbol_table_lookup_in_current_scope(name) != NULL) {
        // Error will be reported by semantic analyzer with line numbers
        return NULL; // Indicate failure due to re-declaration
    }

    unsigned int index = hash_function(name);
    SymbolTableEntry *new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    if (!new_entry) {
        perror("Failed to allocate symbol table entry");
        return NULL; // Or exit
    }

    new_entry->name = strdup(name); // Make a copy of the name
    if (!new_entry->name) {
        perror("Failed to strdup name for symbol entry");
        free(new_entry);
        return NULL;
    }
    new_entry->kind = kind;
    new_entry->type_node = type_node; // Reference to AST node for type
    new_entry->scope_level = current_scope_ptr->level;
    new_entry->line_declared = line_declared;

    // Initialize vulnerability analysis fields
    new_entry->is_initialized = 0;
    new_entry->value_range = range_unknown();
    new_entry->is_defined = 0; // For functions
    new_entry->parameters = NULL; // For functions


    // Insert into hash table (prepend to bucket's linked list)
    new_entry->next_in_bucket = current_scope_ptr->hash_table[index];
    current_scope_ptr->hash_table[index] = new_entry;

    // printf("Symbol Table: Inserted '%s' into scope level %d.\n", name, current_scope_ptr->level);
    return new_entry;
}

SymbolTableEntry* symbol_table_lookup(const char *name) {
    Scope *scope_to_search = current_scope_ptr;
    while (scope_to_search) {
        unsigned int index = hash_function(name);
        SymbolTableEntry *entry = scope_to_search->hash_table[index];
        while (entry) {
            if (strcmp(entry->name, name) == 0) {
                return entry; // Found
            }
            entry = entry->next_in_bucket;
        }
        scope_to_search = scope_to_search->parent; // Move to outer scope
    }
    return NULL; // Not found in any accessible scope
}

SymbolTableEntry* symbol_table_lookup_in_current_scope(const char *name) {
    if (!current_scope_ptr) {
        return NULL;
    }
    unsigned int index = hash_function(name);
    SymbolTableEntry *entry = current_scope_ptr->hash_table[index];
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry; // Found
        }
        entry = entry->next_in_bucket;
    }
    return NULL; // Not found in current scope
}
