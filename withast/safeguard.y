%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h" // *** INCLUDE YOUR AST HEADER ***

// External declarations from the lexer
extern int yylex(void);
extern int current_line; // Used for line numbers in AST nodes
extern int current_column;
extern char *yytext;

void yyerror(const char *s);

ASTNode *ast_root = NULL; // Global variable to store the AST root

// %expect 3 // If you want to suppress S/R warnings
%}

// Bison union for semantic values
%union {
    int int_val;        // For NUMBER_LITERAL token
    char *str_val;      // For IDENTIFIER, STRING_LITERAL tokens
    struct ASTNode *node_ptr; // *** For AST nodes ***
    struct ASTNodeList *node_list_ptr; // For lists of AST nodes
}

// Declare token types and their semantic value types
%token <str_val> IDENTIFIER STRING_LITERAL
%token <int_val> NUMBER_LITERAL

%token LET NUMBER_TYPE ARRAY_TYPE TEXT_TYPE OF IF LOOP WHILE DEFINE RETURNS RETURN
%token READ_NUMBER_FN PRINT_NUMBER_FN PRINT_TEXT_FN SIZE_OF
%token PLUS MINUS STAR SLASH PERCENT ASSIGN SEMICOLON
%token EQUAL_EQUAL BANG_EQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON
%token ELSE

// *** Declare non-terminal types that will have AST node values ***
%type <node_ptr> program top_level_item statement block_statement
%type <node_ptr> var_declaration optional_initializer type_specifier
%type <node_ptr> expression_statement if_statement loop_statement return_statement
%type <node_ptr> expression assignment_expression equality_expression relational_expression
%type <node_ptr> additive_expression multiplicative_expression unary_expression
%type <node_ptr> postfix_expression primary_expression lvalue
%type <node_ptr> function_definition
// %type <node_ptr> opt_param_list param_list param_declaration // For function params

%type <node_list_ptr> item_list statement_list argument_expression_list_opt argument_expression_list


// Operator precedence and associativity (remains the same)
%right ASSIGN
%nonassoc EQUAL_EQUAL BANG_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left STAR SLASH PERCENT
%right UMINUS
%left ELSE

%start program

%%

program:
    item_list
    { ast_root = create_program_node($1); /* $$ = ast_root; if program itself is a node */ }
    ;

item_list:
    /* empty */
    { $$ = NULL; }
    | item_list top_level_item
    { $$ = append_to_list($1, $2); }
    ;

top_level_item:
    function_definition
    { $$ = $1; }
    | statement
    { $$ = $1; }
    ;

statement_list:
    /* empty */
    { $$ = NULL; }
    | statement_list statement
    { $$ = append_to_list($1, $2); }
    ;

statement:
    var_declaration         { $$ = $1; }
    | expression_statement  { $$ = $1; }
    | if_statement          { $$ = $1; }
    | loop_statement        { $$ = $1; }
    | return_statement      { $$ = $1; }
    | block_statement       { $$ = $1; }
    | SEMICOLON             { $$ = create_empty_stmt_node(current_line); }
    ;

block_statement:
    LBRACE statement_list RBRACE
    { $$ = create_block_stmt_node($2, yyloc.first_line); } // Use yyloc for better line info
    ;

var_declaration:
    LET IDENTIFIER COLON type_specifier optional_initializer SEMICOLON
    // $2 is str_val (char*), $4 is node_ptr, $5 is node_ptr
    { $$ = create_var_decl_node($2, $4, $5, @1.first_line); /* $2 (IDENTIFIER) is taken by create_var_decl_node */ }
    ;

optional_initializer:
    /* empty */
    { $$ = NULL; }
    | ASSIGN expression
    { $$ = $2; }
    ;

type_specifier:
    NUMBER_TYPE
    { $$ = create_type_spec_node("number", 0, @1.first_line); }
    | TEXT_TYPE LBRACKET NUMBER_LITERAL RBRACKET
    { $$ = create_type_spec_node("text", $3, @1.first_line); }
    | ARRAY_TYPE LBRACKET NUMBER_LITERAL RBRACKET OF NUMBER_TYPE
    { $$ = create_type_spec_node("array", $3, @1.first_line); /* Assuming element type is number */ }
    ;

expression_statement:
    expression SEMICOLON
    { $$ = create_expression_stmt_node($1, @1.first_line); }
    ;

if_statement:
    IF LPAREN expression RPAREN statement
    { $$ = create_if_stmt_node($3, $5, NULL, @1.first_line); }
    | IF LPAREN expression RPAREN statement ELSE statement
    { $$ = create_if_stmt_node($3, $5, $7, @1.first_line); }
    ;

loop_statement:
    LOOP WHILE LPAREN expression RPAREN statement
    { $$ = create_loop_stmt_node($4, $6, @1.first_line); }
    ;

return_statement:
    RETURN expression SEMICOLON
    { $$ = create_return_stmt_node($2, @1.first_line); }
    | RETURN SEMICOLON
    { $$ = create_return_stmt_node(NULL, @1.first_line); }
    ;

expression:
    assignment_expression { $$ = $1; }
    ;

assignment_expression:
    equality_expression { $$ = $1; }
    | lvalue ASSIGN assignment_expression
    { $$ = create_assignment_expr_node($1, $3, @2.first_line); } // Line of ASSIGN token
    ;

equality_expression:
    relational_expression { $$ = $1; }
    | equality_expression EQUAL_EQUAL relational_expression
    { $$ = create_binary_op_node("==", $1, $3, @2.first_line); }
    | equality_expression BANG_EQUAL relational_expression
    { $$ = create_binary_op_node("!=", $1, $3, @2.first_line); }
    ;

relational_expression:
    additive_expression { $$ = $1; }
    | relational_expression LESS additive_expression
    { $$ = create_binary_op_node("<", $1, $3, @2.first_line); }
    | relational_expression GREATER additive_expression
    { $$ = create_binary_op_node(">", $1, $3, @2.first_line); }
    | relational_expression LESS_EQUAL additive_expression
    { $$ = create_binary_op_node("<=", $1, $3, @2.first_line); }
    | relational_expression GREATER_EQUAL additive_expression
    { $$ = create_binary_op_node(">=", $1, $3, @2.first_line); }
    ;

additive_expression:
    multiplicative_expression { $$ = $1; }
    | additive_expression PLUS multiplicative_expression
    { $$ = create_binary_op_node("+", $1, $3, @2.first_line); }
    | additive_expression MINUS multiplicative_expression
    { $$ = create_binary_op_node("-", $1, $3, @2.first_line); }
    ;

multiplicative_expression:
    unary_expression { $$ = $1; }
    | multiplicative_expression STAR unary_expression
    { $$ = create_binary_op_node("*", $1, $3, @2.first_line); }
    | multiplicative_expression SLASH unary_expression
    { $$ = create_binary_op_node("/", $1, $3, @2.first_line); }
    | multiplicative_expression PERCENT unary_expression
    { $$ = create_binary_op_node("%", $1, $3, @2.first_line); }
    ;

unary_expression:
    postfix_expression { $$ = $1; }
    | MINUS unary_expression %prec UMINUS
    { $$ = create_unary_op_node("-", $2, @1.first_line); }
    ;

postfix_expression:
    primary_expression { $$ = $1; }
    | postfix_expression LBRACKET expression RBRACKET
    { $$ = create_array_access_node($1, $3, @2.first_line); } // Line of LBRACKET
    | postfix_expression LPAREN argument_expression_list_opt RPAREN
    { $$ = create_function_call_node($1, $3, @2.first_line); } // Line of LPAREN
    | PRINT_NUMBER_FN LPAREN expression RPAREN
    // Need a way to represent PRINT_NUMBER_FN as a callable entity node if not just IDENTIFIER
    { ASTNode* func_name_node = create_identifier_node(strdup("print_number"), @1.first_line); /* Kludge if PRINT_NUMBER_FN is not an ID */
      $$ = create_function_call_node(func_name_node, create_node_list($3), @1.first_line); }
    | PRINT_TEXT_FN LPAREN expression RPAREN
    { ASTNode* func_name_node = create_identifier_node(strdup("print_text"), @1.first_line);
      $$ = create_function_call_node(func_name_node, create_node_list($3), @1.first_line); }
    ;

argument_expression_list_opt:
    /* empty */ { $$ = NULL; }
    | argument_expression_list { $$ = $1; }
    ;

argument_expression_list:
    assignment_expression
    { $$ = create_node_list($1); }
    | argument_expression_list ',' assignment_expression
    { $$ = append_to_list($1, $3); }
    ;

primary_expression:
    lvalue { $$ = $1; } // lvalue node is passed up
    | NUMBER_LITERAL
    { $$ = create_number_literal_node($1, @1.first_line); }
    | STRING_LITERAL
    { $$ = create_string_literal_node($1, @1.first_line); /* $1 (string) is taken by create_string_literal_node */ }
    | LPAREN expression RPAREN
    { $$ = $2; } // Pass through the inner expression's AST node
    | READ_NUMBER_FN LPAREN RPAREN
    { $$ = create_read_number_call_node(@1.first_line); }
    | SIZE_OF LPAREN IDENTIFIER RPAREN
    { $$ = create_size_of_call_node($3, @1.first_line); /* $3 (IDENTIFIER string) is taken by create_size_of_call_node */ }
    ;
    // PRINT_NUMBER_FN and PRINT_TEXT_FN if standalone values (not calls) would go here.
    // But typically they are called, so handled in postfix_expression.

lvalue:
    IDENTIFIER
    // $1 is str_val (char*) from lexer. create_identifier_node will take ownership.
    { $$ = create_identifier_node($1, @1.first_line); }
    | lvalue LBRACKET expression RBRACKET
    // $1 is node_ptr (lvalue node), $3 is node_ptr (expression node)
    { $$ = create_array_access_node($1, $3, @2.first_line); /* This is an lvalue array access */ }
    ;

function_definition:
    DEFINE IDENTIFIER LPAREN /* TODO: opt_param_list */ RPAREN RETURNS type_specifier block_statement
    // $2 is str_val (char*), $6 is node_ptr (type_spec), $7 is node_ptr (block_stmt)
    { $$ = create_func_def_node($2, /* params_list_node, */ $6, $7, @1.first_line); /* $2 taken by create_func_def_node */ }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error (Line %d, near column %d, token '%s'): %s\n",
            current_line, current_column, yytext, s);
}

int main(int argc, char *argv[]) {
    FILE *inputFile = stdin;
    if (argc > 1) {
        inputFile = fopen(argv[1], "r");
        if (!inputFile) { perror(argv[1]); return 1; }
    }
    extern FILE *yyin;
    yyin = inputFile;

    // Enable Bison debug traces if needed (set YYDEBUG=1 environment variable)
    // extern int yydebug;
    // yydebug = 1; // Or pass -t to bison and then set this.

    printf("Starting parse...\n");
    int parse_result = yyparse(); // Store result of yyparse

    if (parse_result == 0) { // yyparse returns 0 on success
        if (ast_root) {
            printf("\nParsing completed successfully. AST:\n");
            print_ast(ast_root, 0);
            printf("\nFreeing AST...\n");
            free_ast(ast_root);
            ast_root = NULL;
        } else {
            // This case handles successful parsing of an empty input
            // or input with only comments, where ast_root would be NULL.
            printf("\nParsing completed successfully (input was empty or only comments).\n");
        }
    } else { // yyparse returns non-zero on error (1 for syntax, 2 for memory exhaustion)
        fprintf(stderr, "\nParsing failed (yyparse returned %d).\n", parse_result);
        if (ast_root) { // A partial AST might have been built before the error
            fprintf(stderr, "Freeing partially constructed AST due to error...\n");
            free_ast(ast_root);
            ast_root = NULL;
        }
    }

    if (inputFile != stdin) {
        fclose(inputFile);
    }

    return parse_result; // Return the result of yyparse
}
