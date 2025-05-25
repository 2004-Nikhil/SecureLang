// ast.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h" // Include your header

// --- AST Creation Function Implementations ---

// Helper to allocate and initialize a common ASTNode
static ASTNode* allocate_node(ASTNodeType type, int line) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Fatal: Out of memory allocating AST node.\n");
        exit(EXIT_FAILURE); // Or handle error more gracefully
    }
    node->type = type;
    node->line_number = line;
    // Initialize all union members to 0/NULL to be safe
    memset(&node->data, 0, sizeof(node->data));
    return node;
}

// Literals
ASTNode *create_number_literal_node(int value, int line) {
    ASTNode *node = allocate_node(NODE_NUMBER_LITERAL, line);
    node->data.number_literal = value;
    return node;
}

ASTNode *create_string_literal_node(char *value, int line) { // Takes ownership
    ASTNode *node = allocate_node(NODE_STRING_LITERAL, line);
    node->data.string_literal = value; // Assumes value is already strdup'd by lexer/parser
    return node;
}

// Identifier
ASTNode *create_identifier_node(char *name, int line) { // Takes ownership
    ASTNode *node = allocate_node(NODE_IDENTIFIER, line);
    node->data.identifier_name = name; // Assumes name is strdup'd
    return node;
}

// Type Specifiers
ASTNode *create_type_spec_node(const char *base_type, int size, int line) {
    ASTNode *node = allocate_node(NODE_TYPE_SPECIFIER, line);
    node->data.type_spec.base_type_name = strdup(base_type); // Make a copy
    node->data.type_spec.size = size;
    return node;
}

// Expressions
ASTNode *create_binary_op_node(const char *op_str, ASTNode *left, ASTNode *right, int line) {
    ASTNode *node = allocate_node(NODE_BINARY_OP, line);
    strncpy(node->data.binary_op.op, op_str, 2); // Max 2 chars for ops like ==, <=
    node->data.binary_op.op[2] = '\0';
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

ASTNode *create_unary_op_node(const char *op_str, ASTNode *operand, int line) {
    ASTNode *node = allocate_node(NODE_UNARY_OP, line);
    strncpy(node->data.unary_op.op, op_str, 1);
    node->data.unary_op.op[1] = '\0';
    node->data.unary_op.operand = operand;
    return node;
}

ASTNode *create_assignment_expr_node(ASTNode *lvalue, ASTNode *rvalue, int line) {
    ASTNode *node = allocate_node(NODE_ASSIGN_EXPR, line);
    node->data.assignment.lvalue = lvalue;
    node->data.assignment.rvalue = rvalue;
    return node;
}

ASTNode *create_array_access_node(ASTNode *array_expr, ASTNode *index_expr, int line) {
    ASTNode *node = allocate_node(NODE_ARRAY_ACCESS, line);
    node->data.array_access.array_name_or_expr = array_expr;
    node->data.array_access.index_expr = index_expr;
    return node;
}

ASTNode *create_function_call_node(ASTNode *func_expr, ASTNodeList *args, int line) {
    ASTNode *node = allocate_node(NODE_FUNCTION_CALL, line);
    node->data.function_call.function_name_or_expr = func_expr;
    node->data.function_call.arg_list = args;
    return node;
}

ASTNode *create_read_number_call_node(int line) {
    return allocate_node(NODE_READ_NUMBER_CALL, line);
}

ASTNode *create_size_of_call_node(char* array_name, int line) { // Takes ownership
    ASTNode* node = allocate_node(NODE_SIZE_OF_CALL, line);
    node->data.size_of_call.array_identifier_name = array_name;
    return node;
}


// Statements
ASTNode *create_var_decl_node(char *name, ASTNode *type_node, ASTNode *initializer, int line) { // Takes ownership of name
    ASTNode *node = allocate_node(NODE_VAR_DECL, line);
    node->data.var_decl.var_name = name; // Assumes name is strdup'd
    node->data.var_decl.type_node = type_node;
    node->data.var_decl.initializer = initializer; // Can be NULL
    return node;
}

// If assignment is a statement, not an expression (less common for C-like)
ASTNode *create_assign_stmt_node(ASTNode *lvalue, ASTNode *rvalue, int line) {
     ASTNode *node = allocate_node(NODE_ASSIGN_STMT, line);
     node->data.assignment.lvalue = lvalue;
     node->data.assignment.rvalue = rvalue;
     return node;
}


ASTNode *create_expression_stmt_node(ASTNode *expr, int line) {
    ASTNode *node = allocate_node(NODE_EXPRESSION_STMT, line);
    node->data.expression = expr;
    return node;
}

ASTNode *create_if_stmt_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line) {
    ASTNode *node = allocate_node(NODE_IF_STMT, line);
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch; // Can be NULL
    return node;
}

ASTNode *create_loop_stmt_node(ASTNode *condition, ASTNode *body, int line) {
    ASTNode *node = allocate_node(NODE_LOOP_STMT, line);
    node->data.loop_stmt.condition = condition;
    node->data.loop_stmt.body = body;
    return node;
}

ASTNode *create_return_stmt_node(ASTNode *value, int line) {
    ASTNode *node = allocate_node(NODE_RETURN_STMT, line);
    node->data.return_stmt.return_value = value; // Can be NULL
    return node;
}

ASTNode *create_block_stmt_node(ASTNodeList *statements, int line) {
    ASTNode *node = allocate_node(NODE_BLOCK_STMT, line);
    node->data.statement_list = statements;
    return node;
}

ASTNode *create_empty_stmt_node(int line) {
    return allocate_node(NODE_EMPTY_STMT, line);
}


// Top Level / Lists
ASTNode *create_program_node(ASTNodeList *items) {
    ASTNode *node = allocate_node(NODE_PROGRAM, 0); // Program node line might be 0 or 1
    node->data.statement_list = items; // Using statement_list member for simplicity
    return node;
}

ASTNode *create_func_def_node(char *name, /* ASTNodeList* params, */ ASTNode* return_type, ASTNode* body, int line) { // Takes ownership
    ASTNode *node = allocate_node(NODE_FUNC_DEF, line);
    node->data.func_def.func_name = name; // Assumes name is strdup'd
    // node->data.func_def.param_list = params;
    node->data.func_def.return_type_node = return_type;
    node->data.func_def.body = body;
    return node;
}

ASTNodeList *create_node_list(ASTNode *node) {
    ASTNodeList *list = (ASTNodeList *)malloc(sizeof(ASTNodeList));
    if (!list) {
        fprintf(stderr, "Fatal: Out of memory allocating AST node list.\n");
        exit(EXIT_FAILURE);
    }
    list->node = node;
    list->next = NULL;
    return list;
}

// Prepends node to list, returns new head of list
ASTNodeList *append_to_list(ASTNodeList *list_head, ASTNode *node_to_add) {
    ASTNodeList *new_item = create_node_list(node_to_add);
    if (!list_head) {
        return new_item;
    }
    // To append, iterate to the end of the list
    ASTNodeList *current = list_head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_item;
    return list_head; // Head remains the same
}

// Utility to free the AST (Recursive)
void free_ast(ASTNode *node) {
    if (!node) {
        return;
    }

    // Depending on the node type, free specific data and children
    switch (node->type) {
        case NODE_STRING_LITERAL:
            if (node->data.string_literal) free(node->data.string_literal);
            break;
        case NODE_IDENTIFIER:
            if (node->data.identifier_name) free(node->data.identifier_name);
            break;
        case NODE_TYPE_SPECIFIER:
            if (node->data.type_spec.base_type_name) free(node->data.type_spec.base_type_name);
            break;
        case NODE_BINARY_OP:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
        case NODE_UNARY_OP:
            free_ast(node->data.unary_op.operand);
            break;
        case NODE_ASSIGN_EXPR: // or NODE_ASSIGN_STMT
            free_ast(node->data.assignment.lvalue);
            free_ast(node->data.assignment.rvalue);
            break;
        case NODE_ARRAY_ACCESS:
            free_ast(node->data.array_access.array_name_or_expr);
            free_ast(node->data.array_access.index_expr);
            break;
        case NODE_FUNCTION_CALL:
            free_ast(node->data.function_call.function_name_or_expr);
            // Free argument list
            ASTNodeList *arg_curr = node->data.function_call.arg_list;
            while (arg_curr) {
                ASTNodeList *temp = arg_curr;
                free_ast(temp->node);
                arg_curr = temp->next;
                free(temp);
            }
            break;
        case NODE_SIZE_OF_CALL:
            if(node->data.size_of_call.array_identifier_name) free(node->data.size_of_call.array_identifier_name);
            break;
        case NODE_VAR_DECL:
            if (node->data.var_decl.var_name) free(node->data.var_decl.var_name);
            free_ast(node->data.var_decl.type_node);
            free_ast(node->data.var_decl.initializer);
            break;
        case NODE_IF_STMT:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_branch);
            free_ast(node->data.if_stmt.else_branch);
            break;
        case NODE_LOOP_STMT:
            free_ast(node->data.loop_stmt.condition);
            free_ast(node->data.loop_stmt.body);
            break;
        case NODE_RETURN_STMT:
            free_ast(node->data.return_stmt.return_value);
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK_STMT:
            // Free statement list
            ASTNodeList *stmt_curr = node->data.statement_list;
            while (stmt_curr) {
                ASTNodeList *temp = stmt_curr;
                free_ast(temp->node);
                stmt_curr = temp->next;
                free(temp);
            }
            break;
        case NODE_EXPRESSION_STMT:
            free_ast(node->data.expression);
            break;
        case NODE_FUNC_DEF:
            if (node->data.func_def.func_name) free(node->data.func_def.func_name);
            // free param list ASTNodeList
            free_ast(node->data.func_def.return_type_node);
            free_ast(node->data.func_def.body);
            break;
        // Cases for NODE_NUMBER_LITERAL, NODE_READ_NUMBER_CALL, NODE_EMPTY_STMT
        // don't have dynamically allocated parts within their data union members directly
        // or their children (unless you add them).
        default:
            // No specific dynamic data for this node type in its union, or covered by children.
            break;
    }
    free(node); // Free the node itself
}


// Basic AST Printer (for debugging)
void print_ast_node_list(ASTNodeList* list, int indent); // Forward declaration

void print_ast(ASTNode *node, int indent_level) {
    if (!node) {
        return;
    }

    for (int i = 0; i < indent_level; ++i) printf("  ");

    switch (node->type) {
        case NODE_PROGRAM:
            printf("PROGRAM (Line %d)\n", node->line_number);
            print_ast_node_list(node->data.statement_list, indent_level + 1);
            break;
        case NODE_NUMBER_LITERAL:
            printf("NUMBER_LITERAL: %d (Line %d)\n", node->data.number_literal, node->line_number);
            break;
        case NODE_STRING_LITERAL:
            printf("STRING_LITERAL: \"%s\" (Line %d)\n", node->data.string_literal, node->line_number);
            break;
        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s (Line %d)\n", node->data.identifier_name, node->line_number);
            break;
        case NODE_TYPE_SPECIFIER:
            printf("TYPE_SPECIFIER: %s", node->data.type_spec.base_type_name);
            if (strcmp(node->data.type_spec.base_type_name, "text") == 0 || strcmp(node->data.type_spec.base_type_name, "array") == 0) {
                printf("[%d]", node->data.type_spec.size);
            }
            printf(" (Line %d)\n", node->line_number);
            break;
        case NODE_BINARY_OP:
            printf("BINARY_OP: %s (Line %d)\n", node->data.binary_op.op, node->line_number);
            print_ast(node->data.binary_op.left, indent_level + 1);
            print_ast(node->data.binary_op.right, indent_level + 1);
            break;
        case NODE_UNARY_OP:
            printf("UNARY_OP: %s (Line %d)\n", node->data.unary_op.op, node->line_number);
            print_ast(node->data.unary_op.operand, indent_level + 1);
            break;
        case NODE_ASSIGN_EXPR:
            printf("ASSIGN_EXPR (Line %d)\n", node->line_number);
            print_ast(node->data.assignment.lvalue, indent_level + 1);
            print_ast(node->data.assignment.rvalue, indent_level + 1);
            break;
         case NODE_ASSIGN_STMT:
            printf("ASSIGN_STMT (Line %d)\n", node->line_number);
            print_ast(node->data.assignment.lvalue, indent_level + 1);
            print_ast(node->data.assignment.rvalue, indent_level + 1);
            break;
        case NODE_ARRAY_ACCESS:
            printf("ARRAY_ACCESS (Line %d)\n", node->line_number);
            print_ast(node->data.array_access.array_name_or_expr, indent_level + 1);
            print_ast(node->data.array_access.index_expr, indent_level + 1);
            break;
        case NODE_FUNCTION_CALL:
            printf("FUNCTION_CALL (Line %d)\n", node->line_number);
            print_ast(node->data.function_call.function_name_or_expr, indent_level + 1);
            if (node->data.function_call.arg_list) {
                for (int i = 0; i < indent_level + 1; ++i) printf("  ");
                printf("ARGS:\n");
                print_ast_node_list(node->data.function_call.arg_list, indent_level + 2);
            } else {
                 for (int i = 0; i < indent_level + 1; ++i) printf("  ");
                 printf("ARGS: (none)\n");
            }
            break;
        case NODE_READ_NUMBER_CALL:
            printf("READ_NUMBER_CALL (Line %d)\n", node->line_number);
            break;
        case NODE_SIZE_OF_CALL:
            printf("SIZE_OF_CALL: %s (Line %d)\n", node->data.size_of_call.array_identifier_name, node->line_number);
            break;
        case NODE_VAR_DECL:
            printf("VAR_DECL: %s (Line %d)\n", node->data.var_decl.var_name, node->line_number);
            print_ast(node->data.var_decl.type_node, indent_level + 1);
            if (node->data.var_decl.initializer) {
                for (int i = 0; i < indent_level + 1; ++i) printf("  ");
                printf("INITIALIZER:\n");
                print_ast(node->data.var_decl.initializer, indent_level + 2);
            }
            break;
        case NODE_EXPRESSION_STMT:
            printf("EXPRESSION_STMT (Line %d)\n", node->line_number);
            print_ast(node->data.expression, indent_level + 1);
            break;
        case NODE_IF_STMT:
            printf("IF_STMT (Line %d)\n", node->line_number);
            for (int i = 0; i < indent_level + 1; ++i) printf("  ");
            printf("CONDITION:\n");
            print_ast(node->data.if_stmt.condition, indent_level + 2);
            for (int i = 0; i < indent_level + 1; ++i) printf("  ");
            printf("THEN_BRANCH:\n");
            print_ast(node->data.if_stmt.then_branch, indent_level + 2);
            if (node->data.if_stmt.else_branch) {
                for (int i = 0; i < indent_level + 1; ++i) printf("  ");
                printf("ELSE_BRANCH:\n");
                print_ast(node->data.if_stmt.else_branch, indent_level + 2);
            }
            break;
        case NODE_LOOP_STMT:
            printf("LOOP_STMT (Line %d)\n", node->line_number);
            for (int i = 0; i < indent_level + 1; ++i) printf("  ");
            printf("CONDITION:\n");
            print_ast(node->data.loop_stmt.condition, indent_level + 2);
            for (int i = 0; i < indent_level + 1; ++i) printf("  ");
            printf("BODY:\n");
            print_ast(node->data.loop_stmt.body, indent_level + 2);
            break;
        case NODE_RETURN_STMT:
            printf("RETURN_STMT (Line %d)\n", node->line_number);
            if (node->data.return_stmt.return_value) {
                print_ast(node->data.return_stmt.return_value, indent_level + 1);
            } else {
                for (int i = 0; i < indent_level + 1; ++i) printf("  ");
                printf("(no value)\n");
            }
            break;
        case NODE_BLOCK_STMT:
            printf("BLOCK_STMT (Line %d)\n", node->line_number);
            print_ast_node_list(node->data.statement_list, indent_level + 1);
            break;
        case NODE_EMPTY_STMT:
            printf("EMPTY_STMT (Line %d)\n", node->line_number);
            break;
        case NODE_FUNC_DEF:
            printf("FUNC_DEF: %s (Line %d)\n", node->data.func_def.func_name, node->line_number);
            // print param list
            for (int i = 0; i < indent_level + 1; ++i) printf("  "); printf("RETURNS:\n");
            print_ast(node->data.func_def.return_type_node, indent_level + 2);
            for (int i = 0; i < indent_level + 1; ++i) printf("  "); printf("BODY:\n");
            print_ast(node->data.func_def.body, indent_level + 2);
            break;
        default:
            printf("Unknown AST Node Type: %d (Line %d)\n", node->type, node->line_number);
            break;
    }
}

void print_ast_node_list(ASTNodeList* list, int indent_level) {
    ASTNodeList *current = list;
    while (current != NULL) {
        print_ast(current->node, indent_level);
        current = current->next;
    }
}

char* unescape_string(const char* s, int len) {
    char* result = (char*)malloc(len + 1); // Max possible length
    if (!result) {
        perror("malloc failed in unescape_string");
        exit(EXIT_FAILURE);
    }
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (s[i] == '\\' && i + 1 < len) {
            i++; // Move to the character after backslash
            switch (s[i]) {
                case 'n': result[j++] = '\n'; break;
                case 't': result[j++] = '\t'; break;
                case '\\': result[j++] = '\\'; break;
                case '"': result[j++] = '"'; break;
                default: // Invalid escape, keep literal backslash and char
                    result[j++] = '\\';
                    result[j++] = s[i];
                    // Consider if you want to print a warning from here,
                    // or let a later compiler phase handle it.
                    // For now, consistent with previous versions:
                    // fprintf(stderr, "Warning (Line %d might be off): Invalid escape sequence '\\%c'\n", current_line, s[i]);
                    // Note: current_line might not be accurate if called from a different context than lexer.
                    break;
            }
        } else {
            result[j++] = s[i];
        }
    }
    result[j] = '\0';
    return result;
}
