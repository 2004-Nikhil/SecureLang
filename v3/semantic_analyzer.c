#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Not strictly needed if using SAFEGUARD_INT_MIN/MAX

#include "semantic_analyzer.h"
#include "ast.h"
#include "symbol_table.h"

// Global state for the analyzer
static int error_count = 0;
static int warning_count = 0;
static ASTNode* current_function_def_node = NULL; // To check return types within the current function

// --- Simple TypeKind enum for internal use ---
typedef enum {
    TYPE_KIND_NUMBER,
    TYPE_KIND_TEXT,
    TYPE_KIND_ARRAY_OF_NUMBER,
    TYPE_KIND_VOID,
    TYPE_KIND_FUNCTION, // Represents the "type" of a function identifier itself
    TYPE_KIND_ERROR
} TypeKind;

// --- Error and Warning Reporting ---
static void semantic_error(const char *message, int line) {
    fprintf(stderr, "Semantic Error (Line %d): %s\n", line, message);
    error_count++;
}

static void semantic_warning(const char *message, int line) {
    fprintf(stdout, "Security Warning (Line %d): %s\n", line, message);
    warning_count++;
}

// --- Forward Declarations ---
static TypeKind get_type_kind_from_ast_type_node(ASTNode* type_spec_node);
static TypeKind analyze_expression(ASTNode *expr_node);
static void analyze_statement(ASTNode *stmt_node);
static void analyze_var_decl(ASTNode* node);
static void analyze_func_def(ASTNode* node);
static ValueRange get_expression_range_and_taint(ASTNode* expr_node, int* out_is_tainted);

// --- Main Analysis Function ---
int analyze_semantics(ASTNode *program_node) {
    error_count = 0;
    warning_count = 0;
    current_function_def_node = NULL;

    if (!program_node || program_node->type != NODE_PROGRAM) {
        semantic_error("Invalid program structure: No program root node.", program_node ? program_node->line_number : 0);
        return 1;
    }

    symbol_table_init();

    ASTNodeList *current_item_list = program_node->data.statement_list;
    while (current_item_list) {
        if (current_item_list->node) {
            analyze_statement(current_item_list->node); // Top-level items are statements or func_defs
        }
        current_item_list = current_item_list->next;
    }

    symbol_table_destroy();
    printf("Semantic analysis: %d errors, %d warnings found.\n", error_count, warning_count);
    return error_count; // Return number of errors; 0 means success for this phase
}

// --- Type Utility ---
static TypeKind get_type_kind_from_ast_type_node(ASTNode* type_spec_node) {
    if (!type_spec_node || type_spec_node->type != NODE_TYPE_SPECIFIER) {
        // This might be called with NULL for void functions, handle gracefully
        if (!type_spec_node) return TYPE_KIND_VOID; // Convention for void return
        return TYPE_KIND_ERROR;
    }
    if (strcmp(type_spec_node->data.type_spec.base_type_name, "number") == 0) return TYPE_KIND_NUMBER;
    if (strcmp(type_spec_node->data.type_spec.base_type_name, "text") == 0) return TYPE_KIND_TEXT;
    if (strcmp(type_spec_node->data.type_spec.base_type_name, "array") == 0) return TYPE_KIND_ARRAY_OF_NUMBER;
    // Add TYPE_KIND_VOID_TYPE if you have an explicit 'void' type specifier
    return TYPE_KIND_ERROR;
}

// --- Traversal and Analysis Functions ---
static void analyze_statement(ASTNode *stmt_node) {
    if (!stmt_node) return;

    switch (stmt_node->type) {
        case NODE_VAR_DECL:
            analyze_var_decl(stmt_node);
            break;
        case NODE_EXPRESSION_STMT:
            analyze_expression(stmt_node->data.expression);
            break;
        case NODE_IF_STMT: {
            TypeKind cond_type = analyze_expression(stmt_node->data.if_stmt.condition);
            if (cond_type != TYPE_KIND_NUMBER && cond_type != TYPE_KIND_ERROR) {
                semantic_error("If condition must evaluate to a number.", stmt_node->line_number);
            }
            analyze_statement(stmt_node->data.if_stmt.then_branch);
            if (stmt_node->data.if_stmt.else_branch) {
                analyze_statement(stmt_node->data.if_stmt.else_branch);
            }
            break;
        }
        case NODE_LOOP_STMT: {
            TypeKind cond_type = analyze_expression(stmt_node->data.loop_stmt.condition);
            if (cond_type != TYPE_KIND_NUMBER && cond_type != TYPE_KIND_ERROR) {
                semantic_error("Loop condition must evaluate to a number.", stmt_node->line_number);
            }
            analyze_statement(stmt_node->data.loop_stmt.body);
            break;
        }
        case NODE_RETURN_STMT: {
            if (!current_function_def_node) {
                semantic_error("Return statement found outside of a function definition.", stmt_node->line_number);
            } else {
                SymbolTableEntry* func_entry = symbol_table_lookup(current_function_def_node->data.func_def.func_name);
                TypeKind expected_return_type = TYPE_KIND_VOID;

                if (func_entry && func_entry->type_node) {
                    expected_return_type = get_type_kind_from_ast_type_node(func_entry->type_node);
                } else if (func_entry && !func_entry->type_node) { // No explicit return type in ST means void
                    expected_return_type = TYPE_KIND_VOID;
                } else if (current_function_def_node->data.func_def.return_type_node){ // Fallback to AST
                     expected_return_type = get_type_kind_from_ast_type_node(current_function_def_node->data.func_def.return_type_node);
                }


                if (stmt_node->data.return_stmt.return_value) {
                    if (expected_return_type == TYPE_KIND_VOID) {
                        semantic_error("Function declared with no return type (void) cannot return a value.", stmt_node->line_number);
                    }
                    TypeKind actual_return_type = analyze_expression(stmt_node->data.return_stmt.return_value);
                    if (actual_return_type != TYPE_KIND_ERROR && expected_return_type != TYPE_KIND_ERROR) {
                        if (actual_return_type != expected_return_type) {
                            semantic_error("Return type mismatch with function declaration.", stmt_node->line_number);
                        }
                    }
                } else { // return;
                    if (expected_return_type != TYPE_KIND_VOID && expected_return_type != TYPE_KIND_ERROR) { // Only error if func expects non-void
                        semantic_error("Function expects a return value, but return statement has no value.", stmt_node->line_number);
                    }
                }
            }
            break;
        }
        case NODE_BLOCK_STMT: {
            symbol_table_enter_scope();
            ASTNodeList *s_list = stmt_node->data.statement_list;
            while (s_list) {
                if(s_list->node) analyze_statement(s_list->node);
                s_list = s_list->next;
            }
            symbol_table_exit_scope();
            break;
        }
        case NODE_FUNC_DEF:
            analyze_func_def(stmt_node);
            break;
        case NODE_EMPTY_STMT:
            break;
        default:
             fprintf(stderr, "Internal Compiler Error: Unexpected node type %d in analyze_statement at line %d\n", stmt_node->type, stmt_node->line_number);
            break;
    }
}

static TypeKind analyze_expression(ASTNode *expr_node) {
    if (!expr_node) return TYPE_KIND_ERROR;

    int expr_is_tainted = 0; // Will be set by get_expression_range_and_taint

    switch (expr_node->type) {
        case NODE_NUMBER_LITERAL:
            get_expression_range_and_taint(expr_node, &expr_is_tainted); // To set range on node if desired
            return TYPE_KIND_NUMBER;
        case NODE_STRING_LITERAL:
            get_expression_range_and_taint(expr_node, &expr_is_tainted);
            return TYPE_KIND_TEXT;
        case NODE_IDENTIFIER: {
            SymbolTableEntry *entry = symbol_table_lookup(expr_node->data.identifier_name);
            if (!entry) {
                char msg[100]; sprintf(msg, "Identifier '%s' not declared.", expr_node->data.identifier_name);
                semantic_error(msg, expr_node->line_number);
                return TYPE_KIND_ERROR;
            }

            if (entry->kind == SYMBOL_FUNCTION) return TYPE_KIND_FUNCTION;
            if (entry->kind != SYMBOL_VARIABLE) {
                 char msg[100]; sprintf(msg, "'%s' is not a variable.", expr_node->data.identifier_name);
                 semantic_error(msg, expr_node->line_number); return TYPE_KIND_ERROR;
            }

            if (!entry->is_initialized) {
                char msg[100]; sprintf(msg, "Variable '%s' may be used before initialization.", expr_node->data.identifier_name);
                semantic_warning(msg, expr_node->line_number);
            }
            // To set expr_is_tainted correctly for this identifier
            get_expression_range_and_taint(expr_node, &expr_is_tainted);
            return get_type_kind_from_ast_type_node(entry->type_node);
        }
        case NODE_BINARY_OP: {
            TypeKind left_type = analyze_expression(expr_node->data.binary_op.left);
            TypeKind right_type = analyze_expression(expr_node->data.binary_op.right);
            const char* op = expr_node->data.binary_op.op;

            if (left_type == TYPE_KIND_ERROR || right_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;

            ValueRange result_range = get_expression_range_and_taint(expr_node, &expr_is_tainted); // Calculates range and overall taint

            if (strcmp(op, "+") == 0) {
                if (left_type == TYPE_KIND_NUMBER && right_type == TYPE_KIND_NUMBER) {
                    if (result_range.is_known && (result_range.min < SAFEGUARD_INT_MIN || result_range.max > SAFEGUARD_INT_MAX)) {
                        semantic_warning("Potential integer overflow in '+' operation.", expr_node->line_number);
                    }
                    return TYPE_KIND_NUMBER;
                } else if (left_type == TYPE_KIND_TEXT && right_type == TYPE_KIND_TEXT) {
                    return TYPE_KIND_TEXT;
                } else {
                    semantic_error("Type mismatch for '+' operator (number+number or text+text).", expr_node->line_number);
                    return TYPE_KIND_ERROR;
                }
            } else if (strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
                if (left_type != TYPE_KIND_NUMBER || right_type != TYPE_KIND_NUMBER) {
                    char msg[100]; sprintf(msg, "Operands for '%s' must be numbers.", op);
                    semantic_error(msg, expr_node->line_number); return TYPE_KIND_ERROR;
                }
                // Division/Modulo by zero warnings are inside get_expression_range_and_taint -> range_divide/modulo
                if (result_range.is_known && (result_range.min < SAFEGUARD_INT_MIN || result_range.max > SAFEGUARD_INT_MAX)) {
                     if (strcmp(op, "/") != 0 && strcmp(op, "%") != 0) { // Avoid double warning for div/mod if range is extreme
                        char msg[100]; sprintf(msg, "Potential integer overflow/underflow in '%s' operation.", op);
                        semantic_warning(msg, expr_node->line_number);
                    }
                }
                return TYPE_KIND_NUMBER;
            } else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
                if (!((left_type == TYPE_KIND_NUMBER && right_type == TYPE_KIND_NUMBER) ||
                      (left_type == TYPE_KIND_TEXT && right_type == TYPE_KIND_TEXT))) {
                    semantic_error("Operands for '==' or '!=' must be both numbers or both text.", expr_node->line_number);
                    return TYPE_KIND_ERROR;
                }
                return TYPE_KIND_NUMBER;
            } else if (strcmp(op, "<") == 0  || strcmp(op, "<=") == 0 || strcmp(op, ">") == 0  || strcmp(op, ">=") == 0) {
                if (left_type != TYPE_KIND_NUMBER || right_type != TYPE_KIND_NUMBER) {
                    semantic_error("Operands for relational operators (<, <=, >, >=) must be numbers.", expr_node->line_number);
                    return TYPE_KIND_ERROR;
                }
                return TYPE_KIND_NUMBER;
            }
            char msg[100]; sprintf(msg, "Unknown binary operator '%s'.", op);
            semantic_error(msg, expr_node->line_number); return TYPE_KIND_ERROR;
        }
        case NODE_UNARY_OP: {
            TypeKind operand_type = analyze_expression(expr_node->data.unary_op.operand);
            if (operand_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;

            if (strcmp(expr_node->data.unary_op.op, "-") == 0) {
                if (operand_type != TYPE_KIND_NUMBER) {
                    semantic_error("Operand for unary '-' must be a number.", expr_node->line_number);
                    return TYPE_KIND_ERROR;
                }
                ValueRange r_op = get_expression_range_and_taint(expr_node->data.unary_op.operand, &expr_is_tainted); // Get operand's taint
                ValueRange result_range = range_unary_minus(r_op); // Calculate the range
                expr_is_tainted = r_op.is_tainted; // Unary op propagates taint

                if (r_op.is_known && r_op.min == SAFEGUARD_INT_MIN && result_range.min == SAFEGUARD_INT_MIN) { // Check for -INT_MIN overflow
                    semantic_warning("Potential integer overflow with unary '-' on SAFEGUARD_INT_MIN.", expr_node->line_number);
                } else if (result_range.is_known && (result_range.min < SAFEGUARD_INT_MIN || result_range.max > SAFEGUARD_INT_MAX)) {
                     // This case might be covered if range_unary_minus clamps and INT_MIN case is specific
                }
                return TYPE_KIND_NUMBER;
            }
            semantic_error("Unknown unary operator.", expr_node->line_number); return TYPE_KIND_ERROR;
        }
        case NODE_ASSIGN_EXPR: {
            TypeKind lvalue_type = analyze_expression(expr_node->data.assignment.lvalue);
            if (lvalue_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;

            // Check if lvalue is actually assignable (not a literal, etc.)
            ASTNode* lval_node_ptr = expr_node->data.assignment.lvalue;
            if (lval_node_ptr->type != NODE_IDENTIFIER && lval_node_ptr->type != NODE_ARRAY_ACCESS) {
                semantic_error("Left hand side of assignment is not an assignable variable or array element.", expr_node->line_number);
                return TYPE_KIND_ERROR;
            }

            TypeKind rvalue_type = analyze_expression(expr_node->data.assignment.rvalue);
            if (rvalue_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;

            if (lvalue_type != rvalue_type) {
                semantic_error("Type mismatch in assignment expression.", expr_node->line_number);
                return TYPE_KIND_ERROR;
            }

            SymbolTableEntry* lval_sym_entry = NULL;
            if (lval_node_ptr->type == NODE_IDENTIFIER) {
                lval_sym_entry = symbol_table_lookup(lval_node_ptr->data.identifier_name);
            } else if (lval_node_ptr->type == NODE_ARRAY_ACCESS && lval_node_ptr->data.array_access.array_name_or_expr->type == NODE_IDENTIFIER) {
                lval_sym_entry = symbol_table_lookup(lval_node_ptr->data.array_access.array_name_or_expr->data.identifier_name);
            }

            if (lval_sym_entry) {
                int rval_tainted = 0;
                ValueRange rval_range = get_expression_range_and_taint(expr_node->data.assignment.rvalue, &rval_tainted);
                expr_is_tainted = rval_tainted; // Assignment expression can be tainted if RHS is

                if (lval_node_ptr->type == NODE_IDENTIFIER) { // Direct assignment to variable
                    lval_sym_entry->is_initialized = 1;
                    lval_sym_entry->value_range = rval_range;
                    if(rval_tainted) lval_sym_entry->value_range.is_tainted = 1;
                } else { // Assignment to array element
                    lval_sym_entry->is_initialized = 1; // Mark base array as (partially) initialized
                    if(rval_tainted) lval_sym_entry->value_range.is_tainted = 1; // Taint whole array for simplicity
                }

                if (lvalue_type == TYPE_KIND_TEXT && expr_node->data.assignment.rvalue->type == NODE_STRING_LITERAL) {
                    if (lval_sym_entry->type_node && lval_sym_entry->type_node->type == NODE_TYPE_SPECIFIER) {
                        int max_len = lval_sym_entry->type_node->data.type_spec.size;
                        const char* str_lit = expr_node->data.assignment.rvalue->data.string_literal;
                        if (str_lit && max_len >= 0 && strlen(str_lit) > (size_t)max_len) {
                            semantic_warning("String literal assigned to text variable exceeds its max length.", expr_node->line_number);
                        }
                    }
                }
            }
            return lvalue_type;
        }
        case NODE_ARRAY_ACCESS: {
            ASTNode* array_base_node = expr_node->data.array_access.array_name_or_expr;
            TypeKind base_type = analyze_expression(array_base_node);
            SymbolTableEntry* array_entry = NULL;
            int array_declared_size = -1;

            if (base_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;

            if (array_base_node->type == NODE_IDENTIFIER) {
                array_entry = symbol_table_lookup(array_base_node->data.identifier_name);
                if (!array_entry) return TYPE_KIND_ERROR; // Already caught
                 if (!(array_entry->type_node && get_type_kind_from_ast_type_node(array_entry->type_node) == TYPE_KIND_ARRAY_OF_NUMBER )) {
                    semantic_error("Identifier is not an array, cannot be indexed.", expr_node->line_number); return TYPE_KIND_ERROR;
                }
                array_declared_size = array_entry->type_node->data.type_spec.size;
                if (!array_entry->is_initialized) {
                    // semantic_warning("Array variable might be used before any element is assigned.", expr_node->line_number);
                }
                expr_is_tainted = array_entry->value_range.is_tainted; // Element inherits array's taint status (simplistic)
            } else {
                semantic_error("Array access base must be a simple array identifier.", expr_node->line_number);
                return TYPE_KIND_ERROR;
            }

            TypeKind index_type = analyze_expression(expr_node->data.array_access.index_expr);
            if (index_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;
            if (index_type != TYPE_KIND_NUMBER) {
                semantic_error("Array index must be a number.", expr_node->line_number); return TYPE_KIND_ERROR;
            }

            int index_is_tainted = 0;
            ValueRange index_range = get_expression_range_and_taint(expr_node->data.array_access.index_expr, &index_is_tainted);
            if (index_is_tainted) {
                char msg[150]; sprintf(msg, "Tainted data used as array index for '%s'.", array_entry ? array_entry->name : "array");
                semantic_warning(msg, expr_node->line_number);
            }

            if (array_declared_size != -1) {
                if (expr_node->data.array_access.index_expr->type == NODE_NUMBER_LITERAL) {
                    int index_val = expr_node->data.array_access.index_expr->data.number_literal;
                    if (index_val < 0 || index_val >= array_declared_size) {
                        char msg[150]; sprintf(msg, "Array index %d out of bounds for array '%s' of size %d [0..%d].", index_val, array_entry ? array_entry->name : "array", array_declared_size, array_declared_size -1 );
                        semantic_warning(msg, expr_node->line_number);
                    }
                } else if (index_range.is_known) {
                    if (index_range.min < 0 || index_range.max >= array_declared_size) {
                         char msg[200]; sprintf(msg, "Potential array index out of bounds for array '%s'. Index range [%lld, %lld], bounds [0..%d].", array_entry ? array_entry->name : "array", index_range.min, index_range.max, array_declared_size - 1);
                         semantic_warning(msg, expr_node->line_number);
                    }
                }
            }
            return TYPE_KIND_NUMBER;
        }
        case NODE_FUNCTION_CALL: {
            ASTNode* func_name_node = expr_node->data.function_call.function_name_or_expr;
            TypeKind func_base_type = analyze_expression(func_name_node); // This will resolve IDENTIFIER to TYPE_FUNCTION or error

            if (func_base_type == TYPE_KIND_ERROR) return TYPE_KIND_ERROR;

            if (func_name_node->type == NODE_IDENTIFIER) { // Standard function call by name
                const char* func_name_str = func_name_node->data.identifier_name;
                SymbolTableEntry* func_entry = symbol_table_lookup(func_name_str);

                if (!func_entry || func_entry->kind != SYMBOL_FUNCTION) {
                     char msg[100]; sprintf(msg, "Identifier '%s' is not a function or not defined.", func_name_str);
                     semantic_error(msg, expr_node->line_number); return TYPE_KIND_ERROR;
                }
                // TODO: Argument count/type checking using func_entry->parameters and expr_node->data.function_call.arg_list
                // For now, just analyze arguments for their own errors/taint
                int any_arg_tainted = 0;
                ASTNodeList* current_arg = expr_node->data.function_call.arg_list;
                while(current_arg) {
                    if (current_arg->node) {
                        analyze_expression(current_arg->node); // Analyze arg for errors/warnings
                        int arg_node_taint = 0;
                        get_expression_range_and_taint(current_arg->node, &arg_node_taint);
                        if (arg_node_taint) any_arg_tainted = 1;
                    }
                    current_arg = current_arg->next;
                }
                expr_is_tainted = any_arg_tainted; // Simplistic: call result is tainted if any arg is

                return get_type_kind_from_ast_type_node(func_entry->type_node); // Return type from ST
            } else { // Calling something that is not a simple identifier (e.g. (func_ptr_var)() - not in Safeguard)
                 semantic_error("Function call base must be an identifier.", expr_node->line_number);
                 return TYPE_KIND_ERROR;
            }
        }
        case NODE_READ_NUMBER_CALL:
            expr_is_tainted = 1; // Result of read_number is tainted
            return TYPE_KIND_NUMBER;
        case NODE_SIZE_OF_CALL: {
            SymbolTableEntry* entry = symbol_table_lookup(expr_node->data.size_of_call.array_identifier_name);
            if (!entry || !(entry->type_node && get_type_kind_from_ast_type_node(entry->type_node) == TYPE_KIND_ARRAY_OF_NUMBER )) {
                char msg[100]; sprintf(msg, "Argument to size_of ('%s') must be a declared array.", expr_node->data.size_of_call.array_identifier_name);
                semantic_error(msg, expr_node->line_number); return TYPE_KIND_ERROR;
            }
            return TYPE_KIND_NUMBER;
        }
        default:
            fprintf(stderr, "Internal Compiler Error: Unknown AST node type %d in analyze_expression at line %d\n", expr_node->type, expr_node->line_number);
            exit(EXIT_FAILURE);
    }
    // If you have a way to annotate expr_node with its taint status:
    // if (expr_node) expr_node->actual_is_tainted_field = expr_is_tainted;
    return TYPE_KIND_ERROR; // Should be covered by cases
}


static void analyze_var_decl(ASTNode* node) {
    if (node->data.var_decl.type_node->type == NODE_TYPE_SPECIFIER) {
        if (strcmp(node->data.var_decl.type_node->data.type_spec.base_type_name, "array") == 0 ||
            strcmp(node->data.var_decl.type_node->data.type_spec.base_type_name, "text") == 0) {
            if (node->data.var_decl.type_node->data.type_spec.size <= 0) {
                semantic_error("Array/Text size must be positive.", node->data.var_decl.type_node->line_number);
            }
        }
    }

    SymbolTableEntry *existing = symbol_table_lookup_in_current_scope(node->data.var_decl.var_name);
    if (existing) {
        char msg[150];
        sprintf(msg, "Variable '%s' redefined in the same scope (previously declared at line %d).",
                node->data.var_decl.var_name, existing->line_declared);
        semantic_error(msg, node->line_number);
        return;
    }

    SymbolTableEntry *new_entry = symbol_table_insert(
        node->data.var_decl.var_name, SYMBOL_VARIABLE,
        node->data.var_decl.type_node, node->line_number);

    if (!new_entry) return;

    if (node->data.var_decl.initializer) {
        TypeKind init_type = analyze_expression(node->data.var_decl.initializer);
        TypeKind var_declared_type = get_type_kind_from_ast_type_node(node->data.var_decl.type_node);

        if (init_type != TYPE_KIND_ERROR && var_declared_type != TYPE_KIND_ERROR) {
            if (init_type != var_declared_type) {
                // Allow assigning number to text if you had implicit conversion, but Safeguard is strict
                // For arrays, whole array initialization isn't directly supported by assigning a single expression
                 if (var_declared_type == TYPE_KIND_ARRAY_OF_NUMBER) {
                     semantic_error("Cannot initialize entire array with a single expression in declaration.", node->line_number);
                 } else {
                    semantic_error("Type mismatch in variable initializer.", node->line_number);
                 }
            }
        }
        new_entry->is_initialized = 1;
        int init_is_tainted = 0;
        new_entry->value_range = get_expression_range_and_taint(node->data.var_decl.initializer, &init_is_tainted);
        if(init_is_tainted) new_entry->value_range.is_tainted = 1;


        if (var_declared_type == TYPE_KIND_TEXT && node->data.var_decl.initializer->type == NODE_STRING_LITERAL) {
            if (node->data.var_decl.type_node && node->data.var_decl.type_node->type == NODE_TYPE_SPECIFIER) {
                int max_len = node->data.var_decl.type_node->data.type_spec.size;
                const char* literal_val = node->data.var_decl.initializer->data.string_literal;
                if (literal_val && max_len >= 0 && strlen(literal_val) > (size_t)max_len) {
                    semantic_warning("String literal initializer exceeds declared max length of text variable.", node->line_number);
                }
            }
        }
    } else {
        new_entry->is_initialized = 0;
        new_entry->value_range = range_unknown();
    }
}

static void analyze_func_def(ASTNode* node) {
    // First, try to insert/find the function in the PARENT scope (usually global)
    Scope* parent_scope = symbol_table_current_scope()->parent; // Assuming we are about to enter func scope
    Scope* original_current_scope = symbol_table_current_scope(); // Save current scope before potentially changing it

    SymbolTableEntry* func_entry = NULL;
    if (parent_scope) { // If not already in global, add to parent
        // Temporarily switch to parent scope to insert function symbol
        // This is a bit tricky; cleaner if all funcs are global or symbol_table_insert takes a scope
        // For now, assume functions are declared in the scope *containing* their definition block
        // which is typically the global scope or an outer function's scope (not supported yet).
        // Let's assume symbol_table_insert adds to current_scope_ptr, which should be global here.
    }

    func_entry = symbol_table_lookup_in_current_scope(node->data.func_def.func_name);
    if (func_entry && func_entry->is_defined) {
        char msg[150]; sprintf(msg, "Function '%s' redefined (previously defined at line %d).", node->data.func_def.func_name, func_entry->line_declared);
        semantic_error(msg, node->line_number);
    } else if (func_entry && func_entry->kind != SYMBOL_FUNCTION) {
         char msg[150]; sprintf(msg, "'%s' previously declared as a non-function at line %d.", node->data.func_def.func_name, func_entry->line_declared);
        semantic_error(msg, node->line_number);
        func_entry = NULL; // Treat as if not found for further processing
    }


    if (!func_entry) {
        func_entry = symbol_table_insert(
            node->data.func_def.func_name, SYMBOL_FUNCTION,
            node->data.func_def.return_type_node, node->line_number);
    }
    // else: It was a forward declaration, now we are defining it. Check signature compatibility (TODO).

    if (func_entry) {
        func_entry->is_defined = 1;
        // func_entry->parameters = node->data.func_def.param_list_ast; // Store AST for params
    }

    ASTNode* old_current_function_node = current_function_def_node;
    current_function_def_node = node;

    symbol_table_enter_scope(); // New scope for parameters and locals

    // TODO: Add parameters from node->data.func_def.param_list (if you add it to AST)
    // to the new current scope. For each param:
    //   SymbolTableEntry* param_sym = symbol_table_insert(param_name, SYMBOL_VARIABLE, param_type, param_line);
    //   if (param_sym) param_sym->is_initialized = 1; // Params are considered initialized

    analyze_statement(node->data.func_def.body);

    symbol_table_exit_scope();
    current_function_def_node = old_current_function_node;
}


static ValueRange get_expression_range_and_taint(ASTNode* expr_node, int* out_is_tainted) {
    if (!expr_node) {
        if(out_is_tainted) *out_is_tainted = 0;
        return range_unknown();
    }
    if(out_is_tainted) *out_is_tainted = 0;

    ValueRange vr = range_unknown();
    switch(expr_node->type) {
        case NODE_NUMBER_LITERAL:
            vr = range_const(expr_node->data.number_literal);
            break;
        case NODE_IDENTIFIER: {
            SymbolTableEntry* entry = symbol_table_lookup(expr_node->data.identifier_name);
            if (entry && entry->kind == SYMBOL_VARIABLE) {
                vr = entry->value_range;
                if(out_is_tainted) *out_is_tainted = vr.is_tainted;
            }
            break;
        }
        case NODE_BINARY_OP: {
            int lt=0, rt=0;
            ValueRange r1 = get_expression_range_and_taint(expr_node->data.binary_op.left, &lt); // Corrected
            ValueRange r2 = get_expression_range_and_taint(expr_node->data.binary_op.right, &rt); // Corrected
            if(out_is_tainted) *out_is_tainted = lt || rt;

            const char* op = expr_node->data.binary_op.op;
            if (strcmp(op, "+") == 0) vr = range_add(r1, r2);
            else if (strcmp(op, "-") == 0) vr = range_subtract(r1, r2);
            else if (strcmp(op, "*") == 0) vr = range_multiply(r1, r2);
            else if (strcmp(op, "/") == 0) {
                vr = range_divide(r1, r2);
                if (r2.is_known && r2.min == 0 && r2.max == 0) {
                    semantic_warning("Potential division by zero.", expr_node->line_number);
                } else if (r2.is_known && r2.min <= 0 && r2.max >=0 ) {
                     semantic_warning("Potential division by zero (divisor range includes 0).", expr_node->line_number);
                }
            } else if (strcmp(op, "%") == 0) {
                vr = range_modulo(r1, r2);
                if (r2.is_known && r2.min == 0 && r2.max == 0) {
                    semantic_warning("Potential modulo by zero.", expr_node->line_number);
                } else if (r2.is_known && r2.min <= 0 && r2.max >=0 ) {
                     semantic_warning("Potential modulo by zero (divisor range includes 0).", expr_node->line_number);
                }
            } else { // Comparisons
                vr = range_const(1); // Result is boolean-like (0 or 1), range [0,1] or [1,1]
                if (out_is_tainted) *out_is_tainted = 0; // Comparison results are usually not "data tainted"
            }
            if (vr.is_known && out_is_tainted) vr.is_tainted = *out_is_tainted;
            break;
        }
        case NODE_UNARY_OP: {
            int opt=0;
            ValueRange r_op = get_expression_range_and_taint(expr_node->data.unary_op.operand, &opt);
            if(out_is_tainted) *out_is_tainted = opt;
            if (strcmp(expr_node->data.unary_op.op, "-") == 0) {
                vr = range_unary_minus(r_op);
            }
            if (vr.is_known && out_is_tainted) vr.is_tainted = *out_is_tainted;
            break;
        }
        case NODE_READ_NUMBER_CALL:
            vr = range_unknown();
            vr.is_tainted = 1;
            if(out_is_tainted) *out_is_tainted = 1;
            break;
        case NODE_SIZE_OF_CALL: {
            SymbolTableEntry* entry = symbol_table_lookup(expr_node->data.size_of_call.array_identifier_name);
            if (entry && entry->type_node && get_type_kind_from_ast_type_node(entry->type_node) == TYPE_KIND_ARRAY_OF_NUMBER) {
                vr = range_const(entry->type_node->data.type_spec.size);
            }
            break;
        }
        case NODE_ARRAY_ACCESS: {
            ASTNode* array_base_node = expr_node->data.array_access.array_name_or_expr;
            if (array_base_node->type == NODE_IDENTIFIER) {
                SymbolTableEntry* entry = symbol_table_lookup(array_base_node->data.identifier_name);
                if (entry && out_is_tainted) *out_is_tainted = entry->value_range.is_tainted;
            }
            vr = range_unknown();
            if (vr.is_known && out_is_tainted) vr.is_tainted = *out_is_tainted;
            break;
        }
        case NODE_FUNCTION_CALL: {
            vr = range_unknown();
            ASTNodeList* args = expr_node->data.function_call.arg_list;
            int any_arg_tainted_flag = 0;
            while(args) {
                if (args->node) {
                    int current_arg_node_is_tainted = 0;
                    get_expression_range_and_taint(args->node, &current_arg_node_is_tainted);
                    if (current_arg_node_is_tainted) {
                        any_arg_tainted_flag = 1;
                    }
                }
                args = args->next;
            }
            if(out_is_tainted) *out_is_tainted = any_arg_tainted_flag;
            // Could check if function is a known sanitizer or source here
            if (expr_node->data.function_call.function_name_or_expr->type == NODE_IDENTIFIER &&
                strcmp(expr_node->data.function_call.function_name_or_expr->data.identifier_name, "read_number") == 0) {
                // This specific case should be NODE_READ_NUMBER_CALL, but if it was a generic call:
                if(out_is_tainted) *out_is_tainted = 1;
                vr.is_tainted = 1;
            }

            if (vr.is_known && out_is_tainted) vr.is_tainted = *out_is_tainted;
            break;
        }
        // If LPAREN expression RPAREN in parser is { $$ = $2; }, then the node type is the inner expression's type.
        // No specific case for a "grouped expression node" is needed here.
        default:
            vr = range_unknown();
            break;
    }
    return vr;
}
