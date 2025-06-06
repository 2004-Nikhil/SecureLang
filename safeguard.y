%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h" // For ASTNode, ASTNodeList, and create_..._node functions
#include "semantic_analyzer.h" // For analyze_semantics (will be called in main)
#include "ir_generator.h"
#include "ir_optimizer.h"
#include "ir_core.h"
#include "bytecode_generator.h"
#include "bytecode_opcodes.h"
// External declarations from the lexer
extern FILE *yyin;
extern int yylex(void);
extern char *yytext;     // Current token text for error messages
extern int yylineno;     // Current line number from Flex (if %option yylineno is used)

void yyerror(const char *s); // Bison's error reporting function

ASTNode *ast_root = NULL; // Global variable to store the root of the AST

// %locations // Commented out as we are explicitly passing line numbers via yylval
// %expect 3  // Optional: If you have 3 known benign S/R conflicts and want to suppress warnings
%}

// Bison union for semantic values of tokens and non-terminals
%union {
    int int_val;        // For NUMBER_LITERAL token's integer value
    char *str_val;      // For IDENTIFIER, STRING_LITERAL token's string value (owned by AST after creation)

    // Structures to explicitly carry line numbers with token values
    struct {
        char *s_val;    // String value
        int line;       // Line number of the token
    } token_str_loc;

    struct {
        int i_val;      // Integer value
        int line;       // Line number of the token
    } token_int_loc;

    struct {            // For tokens that only need their line number (keywords, operators)
        int line;
    } token_loc;

    struct ASTNode *node_ptr;        // For rules that produce a single AST node
    struct ASTNodeList *node_list_ptr; // For rules that produce a list of AST nodes
}

// Declare token types and associate them with the correct yylval union member
%token <token_str_loc> IDENTIFIER STRING_LITERAL
%token <token_int_loc> NUMBER_LITERAL

// For keywords and operators, associate with token_loc to get their line numbers
%token <token_loc> LET IF ELSE LOOP WHILE DEFINE RETURNS RETURN OF SEMICOLON
%token <token_loc> NUMBER_TYPE ARRAY_TYPE TEXT_TYPE
%token <token_loc> READ_NUMBER_FN PRINT_NUMBER_FN PRINT_TEXT_FN SIZE_OF
%token <token_loc> PLUS MINUS STAR SLASH PERCENT ASSIGN
%token <token_loc> EQUAL_EQUAL BANG_EQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token <token_loc> LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON

// Declare non-terminal types that will have AST node or node list values
%type <node_ptr> program top_level_item statement block_statement
%type <node_ptr> var_declaration optional_initializer type_specifier
%type <node_ptr> expression_statement if_statement loop_statement return_statement
%type <node_ptr> expression assignment_expression equality_expression relational_expression
%type <node_ptr> additive_expression multiplicative_expression unary_expression
%type <node_ptr> postfix_expression primary_expression lvalue
%type <node_ptr> function_definition
// %type <node_ptr> opt_param_list param_list param_declaration // For function params (if detailed)

%type <node_list_ptr> item_list statement_list argument_expression_list_opt argument_expression_list


// Operator precedence and associativity
%right ASSIGN
%nonassoc EQUAL_EQUAL BANG_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left STAR SLASH PERCENT
%right UMINUS
%left ELSE        // To resolve dangling else

// Start symbol of the grammar
%start program

%%

program:
    item_list
    { $$ = create_program_node($1); ast_root = $$; } // $1 is node_list_ptr
    ;

item_list:
    /* empty */
    { $$ = NULL; }
    | item_list top_level_item
    { $$ = append_to_list($1, $2); } // $1 is node_list_ptr, $2 is node_ptr
    ;

top_level_item:
    function_definition   { $$ = $1; }
    | statement           { $$ = $1; } // statement can be var_declaration
    ;

statement_list: // For statements inside blocks { ... }
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
    | SEMICOLON             { $$ = create_empty_stmt_node($1.line); } // $1 is SEMICOLON token (token_loc)
    ;

block_statement:
    LBRACE statement_list RBRACE
    { $$ = create_block_stmt_node($2, $1.line); } // $1 is LBRACE token (token_loc), $2 is node_list_ptr
    ;

var_declaration:
    LET IDENTIFIER COLON type_specifier optional_initializer SEMICOLON
    // $1 is LET (token_loc), $2 is IDENTIFIER (token_str_loc)
    // $4 is type_specifier (node_ptr), $5 is optional_initializer (node_ptr)
    { $$ = create_var_decl_node($2.s_val, $4, $5, $1.line); /* create_var_decl_node takes ownership of $2.s_val */ }
    ;

optional_initializer:
    /* empty */             { $$ = NULL; }
    | ASSIGN expression     { $$ = $2; } // $2 is expression (node_ptr)
    ;

type_specifier:
    NUMBER_TYPE
    { $$ = create_type_spec_node("number", 0, $1.line); } // $1 is NUMBER_TYPE token (token_loc)
    | TEXT_TYPE LBRACKET NUMBER_LITERAL RBRACKET
    { $$ = create_type_spec_node("text", $3.i_val, $1.line); } // $1 is TEXT_TYPE, $3 is NUMBER_LITERAL (token_int_loc)
    | ARRAY_TYPE LBRACKET NUMBER_LITERAL RBRACKET OF NUMBER_TYPE
    { $$ = create_type_spec_node("array", $3.i_val, $1.line); } // $1 is ARRAY_TYPE, $5 is NUMBER_TYPE (token_loc for of num_type)
    ;

expression_statement:
    expression SEMICOLON
    { $$ = create_expression_stmt_node($1, $1->line_number); } // Use line from already created expression node
    ;

if_statement:
    IF LPAREN expression RPAREN statement
    { $$ = create_if_stmt_node($3, $5, NULL, $1.line); } // $1 is IF token (token_loc)
    | IF LPAREN expression RPAREN statement ELSE statement
    { $$ = create_if_stmt_node($3, $5, $7, $1.line); }
    ;

loop_statement:
    LOOP WHILE LPAREN expression RPAREN statement
    { $$ = create_loop_stmt_node($4, $6, $1.line); } // $1 is LOOP token (token_loc)
    ;

return_statement:
    RETURN expression SEMICOLON
    { $$ = create_return_stmt_node($2, $1.line); } // $1 is RETURN token (token_loc)
    | RETURN SEMICOLON
    { $$ = create_return_stmt_node(NULL, $1.line); }
    ;

expression:
    assignment_expression   { $$ = $1; }
    ;

assignment_expression:
    equality_expression     { $$ = $1; }
    | lvalue ASSIGN assignment_expression
    { $$ = create_assignment_expr_node($1, $3, $2.line); } // $2 is ASSIGN token (token_loc)
    ;

equality_expression:
    relational_expression   { $$ = $1; }
    | equality_expression EQUAL_EQUAL relational_expression
    { $$ = create_binary_op_node("==", $1, $3, $2.line); } // $2 is EQUAL_EQUAL token
    | equality_expression BANG_EQUAL relational_expression
    { $$ = create_binary_op_node("!=", $1, $3, $2.line); } // $2 is BANG_EQUAL token
    ;

relational_expression:
    additive_expression     { $$ = $1; }
    | relational_expression LESS additive_expression
    { $$ = create_binary_op_node("<", $1, $3, $2.line); } // $2 is LESS token
    | relational_expression GREATER additive_expression
    { $$ = create_binary_op_node(">", $1, $3, $2.line); }
    | relational_expression LESS_EQUAL additive_expression
    { $$ = create_binary_op_node("<=", $1, $3, $2.line); }
    | relational_expression GREATER_EQUAL additive_expression
    { $$ = create_binary_op_node(">=", $1, $3, $2.line); }
    ;

additive_expression:
    multiplicative_expression { $$ = $1; }
    | additive_expression PLUS multiplicative_expression
    { $$ = create_binary_op_node("+", $1, $3, $2.line); } // $2 is PLUS token
    | additive_expression MINUS multiplicative_expression
    { $$ = create_binary_op_node("-", $1, $3, $2.line); } // $2 is MINUS token
    ;

multiplicative_expression:
    unary_expression        { $$ = $1; }
    | multiplicative_expression STAR unary_expression
    { $$ = create_binary_op_node("*", $1, $3, $2.line); } // $2 is STAR token
    | multiplicative_expression SLASH unary_expression
    { $$ = create_binary_op_node("/", $1, $3, $2.line); }
    | multiplicative_expression PERCENT unary_expression
    { $$ = create_binary_op_node("%", $1, $3, $2.line); }
    ;

unary_expression:
    postfix_expression      { $$ = $1; }
    | MINUS unary_expression %prec UMINUS
    { $$ = create_unary_op_node("-", $2, $1.line); } // $1 is MINUS token
    ;

postfix_expression:
    primary_expression      { $$ = $1; }
    | postfix_expression LBRACKET expression RBRACKET // $1 is node_ptr, $2 is LBRACKET (token_loc)
    { $$ = create_array_access_node($1, $3, $2.line); /* Line of LBRACKET for the access op */ }
    | postfix_expression LPAREN argument_expression_list_opt RPAREN // $1 is node_ptr, $2 is LPAREN (token_loc)
    { $$ = create_function_call_node($1, $3, $2.line); /* Line of LPAREN for the call op */ }
    // Rules for specific built-in function tokens (if they are not parsed as general IDENTIFIERs)
    | PRINT_NUMBER_FN LPAREN expression RPAREN // $1 is PRINT_NUMBER_FN (token_loc)
    { ASTNode* func_name_node = create_identifier_node(strdup("print_number"), $1.line);
      $$ = create_function_call_node(func_name_node, create_node_list($3), $1.line); }
    | PRINT_TEXT_FN LPAREN expression RPAREN // $1 is PRINT_TEXT_FN (token_loc)
    { ASTNode* func_name_node = create_identifier_node(strdup("print_text"), $1.line);
      $$ = create_function_call_node(func_name_node, create_node_list($3), $1.line); }
    ;

argument_expression_list_opt:
    /* empty */                 { $$ = NULL; }
    | argument_expression_list  { $$ = $1; }
    ;

argument_expression_list:
    assignment_expression       // An argument is any valid expression
    { $$ = create_node_list($1); } // $1 is node_ptr
    | argument_expression_list ',' assignment_expression // $2 is COMMA token (if it had a line in yylval)
    { $$ = append_to_list($1, $3); } // $1 is node_list_ptr, $3 is node_ptr
    ;

primary_expression:
    lvalue          // An lvalue (which will be an IDENTIFIER or ARRAY_ACCESS node) can be used as a primary expression
    { $$ = $1; }    // $1 is node_ptr (already has its line number from lvalue rules)
    | NUMBER_LITERAL
    { $$ = create_number_literal_node($1.i_val, $1.line); } // $1 is NUMBER_LITERAL (token_int_loc)
    | STRING_LITERAL
    { $$ = create_string_literal_node($1.s_val, $1.line); /* $1 (token_str_loc), create_string_literal_node takes ownership of s_val */ }
    | LPAREN expression RPAREN
    { $$ = $2; if ($$) $$->line_number = $1.line; /* Grouped expr takes line of LPAREN ($1 is token_loc) */ }
    | READ_NUMBER_FN LPAREN RPAREN
    { $$ = create_read_number_call_node($1.line); } // $1 is READ_NUMBER_FN (token_loc)
    | SIZE_OF LPAREN IDENTIFIER RPAREN
    // $1 is SIZE_OF (token_loc), $3 is IDENTIFIER (token_str_loc)
    { $$ = create_size_of_call_node($3.s_val, $1.line); /* create_size_of_call_node takes ownership of $3.s_val */ }
    ;

lvalue:
    IDENTIFIER
    // $1 (IDENTIFIER token) is token_str_loc. create_identifier_node takes ownership of s_val.
    { $$ = create_identifier_node($1.s_val, $1.line); }
    | lvalue LBRACKET expression RBRACKET
    // $1 (lvalue) is node_ptr, $2 is LBRACKET (token_loc)
    { $$ = create_array_access_node($1, $3, $2.line); /* Line of LBRACKET for the array access operation itself */ }
    ;

function_definition:
    DEFINE IDENTIFIER LPAREN /* opt_param_list: Add rules for this if needed */ RPAREN RETURNS type_specifier block_statement
    // $1 is DEFINE (token_loc), $2 is IDENTIFIER (token_str_loc), $6 is node_ptr, $7 is node_ptr
    { $$ = create_func_def_node($2.s_val, /* param_list_node, */ $6, $7, $1.line); /* create_func_def_node takes ownership of $2.s_val */ }
    ;

%%

// C code section (yyerror, main)

void yyerror(const char *s) {
    // yylineno from Flex is the line of the token that caused the error
    fprintf(stderr, "Parser Error (Line %d, near token '%s'): %s\n", yylineno, yytext, s);
}

int main(int argc, char *argv[]) {
    FILE *inputFile = stdin;
    if (argc > 1) {
        inputFile = fopen(argv[1], "r");
        if (!inputFile) {
            perror(argv[1]);
            return 1;
        }
    }
    // 'yyin' is typically declared as 'extern FILE *yyin;' by Flex
    // or you declare it globally if needed by other parts of your code.
    // For now, assume it's available.
    yyin = inputFile;

    printf("Starting parse...\n");
    int parse_result = yyparse();

    if (parse_result == 0) { // yyparse returns 0 on success
        if (ast_root) {
            // Set program node line number more accurately
            if (ast_root->type == NODE_PROGRAM && ast_root->data.statement_list && ast_root->data.statement_list->node) {
                ast_root->line_number = ast_root->data.statement_list->node->line_number;
            } else if (ast_root->type == NODE_PROGRAM) {
                ast_root->line_number = 1; // Default for empty program
            }

            printf("\nParsing completed successfully. AST Construction Phase Complete.\n");
            printf("\nInitial AST (Before Semantic Analysis):\n");
            print_ast(ast_root, 0); // Uncomment to print AST

            printf("\nStarting semantic analysis...\n");
            int semantic_issues = analyze_semantics(ast_root);

            if (semantic_issues == 0) {
                printf("Semantic analysis completed with no fatal errors.\n");

                printf("\nStarting IR generation...\n");
                IRList *generated_ir_list = generate_ir(ast_root); // From your ir_generator.c

                if (generated_ir_list) {
                    printf("\n--- Generated IR (Before Optimization) ---\n");
                    print_ir(generated_ir_list); // From your ir_generator.c

                    printf("\nStarting IR optimization...\n");
                    IRList *optimized_ir_list = optimize_ir(generated_ir_list); // From your ir_optimizer.c

                    printf("\n--- Optimized IR ---\n");
                    print_ir(optimized_ir_list);

                    printf("\nStarting bytecode generation...\n");
                    // *** CORRECTED VARIABLE USAGE HERE ***
                    // Pass the head of the IR list (optimized one if available, else original)
                    // generate_bytecode takes IRList* according to your ir_generator.h for print_ir/free_ir
                    // but the bytecode_generator.c I provided took IRInstruction*.
                    // Let's assume generate_bytecode is adapted or you pass the correct head.
                    // For now, this assumes generate_bytecode can handle IRList*
                    BytecodeChunk bytecode_result = generate_bytecode(optimized_ir_list ? optimized_ir_list : generated_ir_list);

                    if (bytecode_result.code && bytecode_result.count > 0) {
                        printf("Bytecode generation successful. Size: %d bytes\n", bytecode_result.count);
                        print_bytecode_raw(&bytecode_result); // For debugging

                        FILE* outfile = fopen("output.bcs", "wb");
                        if (outfile) {
                            fwrite(bytecode_result.code, sizeof(uint8_t), bytecode_result.count, outfile);
                            // TODO: Write string pool from bytecode_result.string_pool
                            fclose(outfile);
                            printf("Bytecode written to output.bcs\n");
                        } else {
                            perror("Failed to open output bytecode file");
                        }
                        free_bytecode_chunk(&bytecode_result);
                    } else {
                        printf("Bytecode generation failed or produced no bytecode.\n");
                    }

                    // Free IR lists correctly
                    if (optimized_ir_list != generated_ir_list && generated_ir_list != NULL) {
                        // If optimizer returns a *new* list and original is different
                        free_ir(generated_ir_list);
                    }
                    if (optimized_ir_list != NULL) { // Always free the final list from optimizer
                        free_ir(optimized_ir_list);
                    }


                } else {
                    printf("IR generation failed or produced no IR.\n");
                }
            } else {
                printf("Semantic analysis found %d fatal errors. Skipping subsequent phases.\n", semantic_issues);
            }

            printf("\nFreeing AST...\n");
            free_ast(ast_root);
            ast_root = NULL;
        } else {
            printf("\nParsing completed successfully (input was empty or only comments, no AST generated).\n");
        }
    } else {
        fprintf(stderr, "\nParsing failed (yyparse returned %d).\n", parse_result);
        if (ast_root) {
            fprintf(stderr, "Freeing partially constructed AST due to parsing error...\n");
            free_ast(ast_root);
            ast_root = NULL;
        }
    }

    if (inputFile != stdin) {
        fclose(inputFile);
    }
    return parse_result;
}
