%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic_analyzer.h"
extern int yylex(void);
// extern int current_line; // No longer need to directly use lexer's current_line global here
// extern int current_column; // Can be used for more precise error messages if needed
extern char *yytext;
extern int yylineno; // If Flex is providing it, Bison can use it for default locations

void yyerror(const char *s);

ASTNode *ast_root = NULL;

// %expect 3 // Keep if the 3 S/R conflicts are confirmed benign
%}

%locations // *** Ensure Bison location tracking is explicitly enabled ***
%union {
    int int_val;
    char *str_val;
    struct ASTNode *node_ptr;
    struct ASTNodeList *node_list_ptr;
}

%token <str_val> IDENTIFIER STRING_LITERAL
%token <int_val> NUMBER_LITERAL

%token LET NUMBER_TYPE ARRAY_TYPE TEXT_TYPE OF IF LOOP WHILE DEFINE RETURNS RETURN
%token READ_NUMBER_FN PRINT_NUMBER_FN PRINT_TEXT_FN SIZE_OF
%token PLUS MINUS STAR SLASH PERCENT ASSIGN SEMICOLON
%token EQUAL_EQUAL BANG_EQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON
%token ELSE

%type <node_ptr> program top_level_item statement block_statement
%type <node_ptr> var_declaration optional_initializer type_specifier
%type <node_ptr> expression_statement if_statement loop_statement return_statement
%type <node_ptr> expression assignment_expression equality_expression relational_expression
%type <node_ptr> additive_expression multiplicative_expression unary_expression
%type <node_ptr> postfix_expression primary_expression lvalue
%type <node_ptr> function_definition

%type <node_list_ptr> item_list statement_list argument_expression_list_opt argument_expression_list

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
    // For program node, use line of first item or 1 if empty
    { $$ = create_program_node($1); ast_root = $$; }
    ;

item_list:
    /* empty */                 { $$ = NULL; }
    | item_list top_level_item  { $$ = append_to_list($1, $2); }
    ;

top_level_item:
    function_definition { $$ = $1; }
    | statement           { $$ = $1; }
    ;

statement_list:
    /* empty */                 { $$ = NULL; }
    | statement_list statement  { $$ = append_to_list($1, $2); }
    ;

statement:
    var_declaration         { $$ = $1; }
    | expression_statement  { $$ = $1; }
    | if_statement          { $$ = $1; }
    | loop_statement        { $$ = $1; }
    | return_statement      { $$ = $1; }
    | block_statement       { $$ = $1; }
    | SEMICOLON             { $$ = create_empty_stmt_node(@1.first_line); } // Line of the semicolon
    ;

block_statement:
    LBRACE statement_list RBRACE
    { $$ = create_block_stmt_node($2, @1.first_line); } // Line of LBRACE
    ;

var_declaration:
    LET IDENTIFIER COLON type_specifier optional_initializer SEMICOLON
    { $$ = create_var_decl_node($2, $4, $5, @1.first_line); } // Line of LET
    ;

optional_initializer:
    /* empty */         { $$ = NULL; }
    | ASSIGN expression { $$ = $2; } // Initializer AST node takes line from expression
    ;

type_specifier:
    NUMBER_TYPE
    { $$ = create_type_spec_node("number", 0, @1.first_line); }
    | TEXT_TYPE LBRACKET NUMBER_LITERAL RBRACKET
    { $$ = create_type_spec_node("text", $3, @1.first_line); }
    | ARRAY_TYPE LBRACKET NUMBER_LITERAL RBRACKET OF NUMBER_TYPE
    { $$ = create_type_spec_node("array", $3, @1.first_line); }
    ;

expression_statement:
    expression SEMICOLON
    { $$ = create_expression_stmt_node($1, $1->line_number); } // Line from the expression node
    ;

if_statement:
    IF LPAREN expression RPAREN statement
    { $$ = create_if_stmt_node($3, $5, NULL, @1.first_line); } // Line of IF
    | IF LPAREN expression RPAREN statement ELSE statement
    { $$ = create_if_stmt_node($3, $5, $7, @1.first_line); } // Line of IF
    ;

loop_statement:
    LOOP WHILE LPAREN expression RPAREN statement
    { $$ = create_loop_stmt_node($4, $6, @1.first_line); } // Line of LOOP
    ;

return_statement:
    RETURN expression SEMICOLON
    { $$ = create_return_stmt_node($2, @1.first_line); } // Line of RETURN
    | RETURN SEMICOLON
    { $$ = create_return_stmt_node(NULL, @1.first_line); } // Line of RETURN
    ;

expression:
    assignment_expression { $$ = $1; }
    ;

assignment_expression:
    equality_expression { $$ = $1; }
    | lvalue ASSIGN assignment_expression
    { $$ = create_assignment_expr_node($1, $3, @2.first_line); } // Line of ASSIGN
    ;

equality_expression:
    relational_expression { $$ = $1; }
    | equality_expression EQUAL_EQUAL relational_expression
    { $$ = create_binary_op_node("==", $1, $3, @2.first_line); } // Line of ==
    | equality_expression BANG_EQUAL relational_expression
    { $$ = create_binary_op_node("!=", $1, $3, @2.first_line); } // Line of !=
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
    { $$ = create_binary_op_node("+", $1, $3, @2.first_line); } // Line of PLUS
    | additive_expression MINUS multiplicative_expression
    { $$ = create_binary_op_node("-", $1, $3, @2.first_line); } // Line of MINUS
    ;

multiplicative_expression:
    unary_expression { $$ = $1; }
    | multiplicative_expression STAR unary_expression
    { $$ = create_binary_op_node("*", $1, $3, @2.first_line); } // Line of STAR
    | multiplicative_expression SLASH unary_expression
    { $$ = create_binary_op_node("/", $1, $3, @2.first_line); } // Line of SLASH
    | multiplicative_expression PERCENT unary_expression
    { $$ = create_binary_op_node("%", $1, $3, @2.first_line); } // Line of PERCENT
    ;

unary_expression:
    postfix_expression { $$ = $1; }
    | MINUS unary_expression %prec UMINUS
    { $$ = create_unary_op_node("-", $2, @1.first_line); } // Line of MINUS
    ;

postfix_expression:
    primary_expression { $$ = $1; }
    | postfix_expression LBRACKET expression RBRACKET
    { $$ = create_array_access_node($1, $3, $1->line_number); } // Line of the base postfix expression
    | postfix_expression LPAREN argument_expression_list_opt RPAREN
    { $$ = create_function_call_node($1, $3, $1->line_number); } // Line of the base postfix expression
    | PRINT_NUMBER_FN LPAREN expression RPAREN
    { ASTNode* func_name_node = create_identifier_node(strdup("print_number"), @1.first_line);
      $$ = create_function_call_node(func_name_node, create_node_list($3), @1.first_line); }
    | PRINT_TEXT_FN LPAREN expression RPAREN
    { ASTNode* func_name_node = create_identifier_node(strdup("print_text"), @1.first_line);
      $$ = create_function_call_node(func_name_node, create_node_list($3), @1.first_line); }
    ;

argument_expression_list_opt:
    /* empty */                 { $$ = NULL; }
    | argument_expression_list  { $$ = $1; }
    ;

argument_expression_list:
    assignment_expression       { $$ = create_node_list($1); }
    | argument_expression_list ',' assignment_expression { $$ = append_to_list($1, $3); }
    ;

primary_expression:
    lvalue { $$ = $1; } // lvalue node already has its line number
    | NUMBER_LITERAL
    { $$ = create_number_literal_node($1, @1.first_line); } // Line of NUMBER_LITERAL token
    | STRING_LITERAL
    { $$ = create_string_literal_node($1, @1.first_line); } // Line of STRING_LITERAL token
    | LPAREN expression RPAREN
    { $$ = $2; $2->line_number = @1.first_line; /* Or $2 takes its own line, grouped expr can take LPAREN's line */ }
    | READ_NUMBER_FN LPAREN RPAREN
    { $$ = create_read_number_call_node(@1.first_line); } // Line of READ_NUMBER_FN token
    | SIZE_OF LPAREN IDENTIFIER RPAREN
    { $$ = create_size_of_call_node($3, @1.first_line); } // Line of SIZE_OF token
    ;

lvalue:
    IDENTIFIER
    { $$ = create_identifier_node($1, @1.first_line); } // Line of IDENTIFIER token
    | lvalue LBRACKET expression RBRACKET
    { $$ = create_array_access_node($1, $3, $1->line_number); } // Line of base lvalue (e.g. for a in a[i])
    ;

function_definition:
    DEFINE IDENTIFIER LPAREN /* opt_param_list */ RPAREN RETURNS type_specifier block_statement
    { $$ = create_func_def_node($2, /* params */ $6, $7, @1.first_line); } // Line of DEFINE
    ;

%%

void yyerror(const char *s) {
    // Use yylineno if Flex is providing it and Bison is using it for default error location
    fprintf(stderr, "Error (Line %d, near token '%s'): %s\n", yylineno, yytext, s);
}

int main(int argc, char *argv[]) {
    FILE *inputFile = stdin;
    if (argc > 1) {
        inputFile = fopen(argv[1], "r");
        if (!inputFile) { perror(argv[1]); return 1; }
    }
    extern FILE *yyin;
    yyin = inputFile;

    // extern int yydebug; // For Bison debug output
    // yydebug = 1;

    printf("Starting parse...\n");
    int parse_result = yyparse();

    if (parse_result == 0) {
    if (ast_root) {
        printf("\nParsing completed successfully. AST:\n");
        print_ast(ast_root, 0);

        printf("\nStarting semantic analysis...\n");
        int issues_found = analyze_semantics(ast_root); // <<<--- CALL IT HERE
        if (issues_found == 0) {
            printf("Semantic analysis completed with no errors or warnings.\n");
            // Proceed to IR generation if no fatal errors
        } else {
            printf("Semantic analysis completed with %d issues.\n", issues_found);
        }

        printf("\nFreeing AST...\n");
        free_ast(ast_root);
        ast_root = NULL;
    }


    /*if (parse_result == 0) {
        if (ast_root) {
            // Set program node line number more accurately if possible
            if (ast_root->data.statement_list && ast_root->data.statement_list->node) {
                ast_root->line_number = ast_root->data.statement_list->node->line_number;
            } else {
                ast_root->line_number = 1; // Default for empty program
            }
            printf("\nParsing completed successfully. AST:\n");
            print_ast(ast_root, 0);
            printf("\nFreeing AST...\n");
            free_ast(ast_root);
            ast_root = NULL;
        } else {
            printf("\nParsing completed successfully (input was empty or only comments).\n");
        }
    } else {
        fprintf(stderr, "\nParsing failed (yyparse returned %d).\n", parse_result);
        if (ast_root) {
            fprintf(stderr, "Freeing partially constructed AST due to error...\n");
            free_ast(ast_root);
            ast_root = NULL;
        }*/
    }

    if (inputFile != stdin) {
        fclose(inputFile);
    }
    return parse_result;
}
