// ast.h
#ifndef AST_H
#define AST_H

// Enum for all possible AST node types
typedef enum {
    // Literals
    NODE_NUMBER_LITERAL,
    NODE_STRING_LITERAL,

    // Identifiers (can be lvalue or rvalue context dependent)
    NODE_IDENTIFIER,

    // Type Specifiers
    NODE_TYPE_SPECIFIER, // Can hold kind (number, text, array) and size for text/array

    // Expressions
    NODE_BINARY_OP,      // For +, -, *, /, %, ==, !=, <, <=, >, >=
    NODE_UNARY_OP,       // For unary - (and potentially others later)
    NODE_ASSIGN_EXPR,
    NODE_ARRAY_ACCESS,   // For a[i] when used as an expression (r-value)
    NODE_FUNCTION_CALL,  // For user functions and built-ins like print_number
    NODE_READ_NUMBER_CALL,
    NODE_SIZE_OF_CALL,

    // LValues (specifically for LHS of assignment or when an addressable entity is needed)
    // Note: Often, IDENTIFIER and ARRAY_ACCESS can serve as lvalues,
    // and the context (assignment) determines their lvalue role.
    // Or, you can have specific LVALUE_IDENTIFIER, LVALUE_ARRAY_ACCESS nodes.
    // For simplicity, let's initially use IDENTIFIER and ARRAY_ACCESS and let semantic
    // analysis check their usage. If required, we can make specific lvalue nodes.

    // Statements
    NODE_VAR_DECL,
    NODE_ASSIGN_STMT,    // May be redundant if assignment is an expression
    NODE_EXPRESSION_STMT,
    NODE_IF_STMT,
    NODE_LOOP_STMT,
    NODE_RETURN_STMT,
    NODE_BLOCK_STMT,
    NODE_EMPTY_STMT,     // For just a semicolon

    // Top Level
    NODE_PROGRAM,        // Root of the AST
    NODE_FUNC_DEF,
    NODE_STATEMENT_LIST, // For sequences of statements in blocks or at top level

    // Other helpful nodes
    NODE_ARG_LIST        // For function call arguments
} ASTNodeType;

// Forward declaration for struct ASTNode
struct ASTNode;

// For lists of nodes (e.g., statement list, argument list)
typedef struct ASTNodeList {
    struct ASTNode *node;
    struct ASTNodeList *next;
} ASTNodeList;


// Generic AST Node Structure
typedef struct ASTNode {
    ASTNodeType type;
    int line_number; // Store the line number for error reporting and warnings

    union {
        // Literals
        int number_literal;
        char *string_literal; // Remember to free this if strdup'd

        // Identifier
        char *identifier_name; // Remember to free this if strdup'd

        // Type Specifier
        struct {
            char* base_type_name; // "number", "text", "array"
            int size;             // For text (max_length), array (num_elements)
            // char* array_element_type; // If arrays could hold other types (not for current Safeguard)
        } type_spec;

        // Binary Operation
        struct {
            char op[3]; // Operator like "+", "==", "<="
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;

        // Unary Operation
        struct {
            char op[2]; // Operator like "-"
            struct ASTNode *operand;
        } unary_op;

        // Assignment (can be an expression or a statement node)
        struct {
            struct ASTNode *lvalue;
            struct ASTNode *rvalue;
        } assignment;

        // Array Access (can be lvalue or rvalue context dependent)
        struct {
            struct ASTNode *array_name_or_expr; // Could be an IDENTIFIER or another expression yielding an array
            struct ASTNode *index_expr;
        } array_access;

        // Function Call
        struct {
            struct ASTNode *function_name_or_expr; // Could be an IDENTIFIER or expr yielding func
            ASTNodeList *arg_list;
        } function_call;

        // Size Of Call
        struct {
            char* array_identifier_name; // Stores the name of the array for size_of
        } size_of_call;


        // Variable Declaration
        struct {
            char *var_name; // Remember to free
            struct ASTNode *type_node;
            struct ASTNode *initializer; // Optional, can be NULL
        } var_decl;

        // If Statement
        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch; // Optional, can be NULL
        } if_stmt;

        // Loop Statement
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } loop_stmt;

        // Return Statement
        struct {
            struct ASTNode *return_value; // Optional, can be NULL
        } return_stmt;

        // Block Statement / Program / Statement List
        ASTNodeList *statement_list;

        // Expression Statement
        struct ASTNode *expression;


        // Function Definition
        struct {
            char *func_name; // Remember to free
            // ASTNodeList *param_list; // TODO: Define parameter list structure
            struct ASTNode *return_type_node;
            struct ASTNode *body; // A NODE_BLOCK_STMT
        } func_def;

    } data;
} ASTNode;


// --- AST Creation Function Prototypes ---
// Literals
ASTNode *create_number_literal_node(int value, int line);
ASTNode *create_string_literal_node(char *value, int line); // Takes ownership of value

// Identifier
ASTNode *create_identifier_node(char *name, int line); // Takes ownership of name

// Type Specifiers
ASTNode *create_type_spec_node(const char *base_type, int size, int line); // e.g. ("number", 0), ("text", 50), ("array", 10)

// Expressions
ASTNode *create_binary_op_node(const char *op, ASTNode *left, ASTNode *right, int line);
ASTNode *create_unary_op_node(const char *op, ASTNode *operand, int line);
ASTNode *create_assignment_expr_node(ASTNode *lvalue, ASTNode *rvalue, int line); // For assignment as expression
ASTNode *create_array_access_node(ASTNode *array_expr, ASTNode *index_expr, int line);
ASTNode *create_function_call_node(ASTNode *func_expr, ASTNodeList *args, int line);
ASTNode *create_read_number_call_node(int line);
ASTNode *create_size_of_call_node(char* array_name, int line); // Takes ownership of array_name

// Statements
ASTNode *create_var_decl_node(char *name, ASTNode *type_node, ASTNode *initializer, int line); // Takes ownership of name
ASTNode *create_assign_stmt_node(ASTNode *lvalue, ASTNode *rvalue, int line); // If assignment is a statement
ASTNode *create_expression_stmt_node(ASTNode *expr, int line);
ASTNode *create_if_stmt_node(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line);
ASTNode *create_loop_stmt_node(ASTNode *condition, ASTNode *body, int line);
ASTNode *create_return_stmt_node(ASTNode *value, int line);
ASTNode *create_block_stmt_node(ASTNodeList *statements, int line);
ASTNode *create_empty_stmt_node(int line);

// Top Level / Lists
ASTNode *create_program_node(ASTNodeList *items); // items are top_level_item nodes
ASTNode *create_func_def_node(char *name, /* ASTNodeList* params, */ ASTNode* return_type, ASTNode* body, int line); // Takes ownership of name
ASTNodeList *append_to_list(ASTNodeList *list, ASTNode *node); // Prepends to list is easier
ASTNodeList *create_node_list(ASTNode *node);


// Utility
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int indent); // For debugging
char* unescape_string(const char* s, int len);
#endif // AST_H
