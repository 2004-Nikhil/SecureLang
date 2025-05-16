%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External declarations from the lexer
extern int yylex(void);
extern int current_line;
extern int current_column;
extern char *yytext;

// Function to report errors (required by Bison)
void yyerror(const char *s);

// AST related code: Uncomment and define these when you start building the AST.
/*
typedef struct ASTNode {
    // Define your AST node structure here
    // char* node_type;
    // ... other fields for children, values, line numbers ...
} ASTNode;
// typedef struct ASTNodeList { ... } ASTNodeList; // For lists if needed
// AST Construction helper function declarations
*/

// %expect 3 // Add this if you want to suppress the 3 known S/R warnings

%}

// Bison union for semantic values
%union {
    int int_val;
    char *str_val;
    // struct ASTNode *node_ptr; // For AST nodes
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

// Declare non-terminal types if building AST
// %type <node_ptr> program item_list top_level_item statement_list statement ... (and so on)


// Operator precedence and associativity
%right ASSIGN
%nonassoc EQUAL_EQUAL BANG_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left STAR SLASH PERCENT
%right UMINUS
%left ELSE

// Start symbol
%start program

%%

program:
    item_list
    { printf("Parser: Program parsed successfully.\n"); /* $$ = $1; (AST root) */ }
    ;

item_list:
    /* empty */
    | item_list top_level_item
    ;

top_level_item:
    function_definition
    | statement       // statement can be var_declaration
    ;

statement_list:
    /* empty */
    | statement_list statement
    ;

statement:
    var_declaration
    | expression_statement
    | if_statement
    | loop_statement
    | return_statement
    | block_statement
    | SEMICOLON
    ;

block_statement:
    LBRACE statement_list RBRACE
    ;

var_declaration:
    LET IDENTIFIER COLON type_specifier optional_initializer SEMICOLON
        { printf("Parser: Variable declaration for '%s'\n", $2); if($2) free($2); }
    ;

optional_initializer:
    /* empty */
    | ASSIGN expression
        { printf("Parser: ...with initializer.\n"); }
    ;

type_specifier:
    NUMBER_TYPE { printf("Parser: Type 'number'.\n"); }
    | TEXT_TYPE LBRACKET NUMBER_LITERAL RBRACKET { printf("Parser: Type 'text' size %d.\n", $3); }
    | ARRAY_TYPE LBRACKET NUMBER_LITERAL RBRACKET OF NUMBER_TYPE { printf("Parser: Type 'array' size %d of number.\n", $3); }
    ;

expression_statement:
    expression SEMICOLON
    ;

if_statement:
    IF LPAREN expression RPAREN statement
        { printf("Parser: If statement (no else).\n"); }
    | IF LPAREN expression RPAREN statement ELSE statement
        { printf("Parser: If-Else statement.\n"); }
    ;

loop_statement:
    LOOP WHILE LPAREN expression RPAREN statement
        { printf("Parser: Loop-While statement.\n"); }
    ;

return_statement:
    RETURN expression SEMICOLON { printf("Parser: Return statement with value.\n"); }
    | RETURN SEMICOLON { printf("Parser: Return statement (void-like).\n"); }
    ;

expression:
    assignment_expression ;

assignment_expression:
    equality_expression
    | lvalue ASSIGN assignment_expression
        { printf("Parser: Assignment Expression (e.g. x = ...).\n"); }
    ;

equality_expression:
    relational_expression
    | equality_expression EQUAL_EQUAL relational_expression { printf("Parser: Expr ==.\n"); }
    | equality_expression BANG_EQUAL relational_expression { printf("Parser: Expr !=.\n"); }
    ;

relational_expression:
    additive_expression
    | relational_expression LESS additive_expression { printf("Parser: Expr <.\n"); }
    | relational_expression GREATER additive_expression { printf("Parser: Expr >.\n"); }
    | relational_expression LESS_EQUAL additive_expression { printf("Parser: Expr <=.\n"); }
    | relational_expression GREATER_EQUAL additive_expression { printf("Parser: Expr >=.\n"); }
    ;

additive_expression:
    multiplicative_expression
    | additive_expression PLUS multiplicative_expression { printf("Parser: Expr Add.\n"); }
    | additive_expression MINUS multiplicative_expression { printf("Parser: Expr Subtract.\n"); }
    ;

multiplicative_expression:
    unary_expression
    | multiplicative_expression STAR unary_expression { printf("Parser: Expr Multiply.\n"); }
    | multiplicative_expression SLASH unary_expression { printf("Parser: Expr Divide.\n"); }
    | multiplicative_expression PERCENT unary_expression { printf("Parser: Expr Modulo.\n"); }
    ;

unary_expression:
    postfix_expression
    | MINUS unary_expression %prec UMINUS { printf("Parser: Expr Unary Minus.\n"); }
    ;

postfix_expression:
    primary_expression
    | postfix_expression LBRACKET expression RBRACKET
        { printf("Parser: Array access from postfix 'postfix_expression [ expression ]'.\n"); }
    | postfix_expression LPAREN argument_expression_list_opt RPAREN // General function call, base is primary_expression
        { printf("Parser: Function call from postfix 'postfix_expression ( args )'.\n"); }
    // Specific rules for built-in function *tokens* if they are not just IDENTIFIERs
    // If PRINT_NUMBER_FN is a token, this is how it would be called:
    | PRINT_NUMBER_FN LPAREN expression RPAREN
        { printf("Parser: Call to PRINT_NUMBER_FN built-in.\n"); }
    | PRINT_TEXT_FN LPAREN expression RPAREN
        { printf("Parser: Call to PRINT_TEXT_FN built-in.\n"); }
    ;

argument_expression_list_opt:
    /* empty */
    | argument_expression_list
    ;

argument_expression_list:
    assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

primary_expression:
    lvalue
    | NUMBER_LITERAL { printf("Parser: Primary NUMBER_LITERAL %d\n", $1); }
    | STRING_LITERAL { printf("Parser: Primary STRING_LITERAL \"%s\"\n", $1); if($1) free($1); }
    | LPAREN expression RPAREN { printf("Parser: Grouped expression.\n"); }
    | READ_NUMBER_FN LPAREN RPAREN { printf("Parser: Primary READ_NUMBER_FN call.\n"); }
    // PRINT_NUMBER_FN and PRINT_TEXT_FN are now called via postfix_expression if they are unique tokens
    // If they were IDENTIFIERs, lvalue->primary_expression->postfix_expression (call) would handle it.
    | SIZE_OF LPAREN IDENTIFIER RPAREN { printf("Parser: Primary SIZE_OF call for '%s'.\n", $3); if($3) free($3); }
    ;

lvalue:
    IDENTIFIER
        // { printf("Parser: lvalue -> IDENTIFIER '%s'\n", $1); /* $1 is str_val. No free here if used by primary_expression or assignment in AST */ }
    | lvalue LBRACKET expression RBRACKET
        { printf("Parser: lvalue -> lvalue [ expr ]\n"); }
    ;

function_definition:
    DEFINE IDENTIFIER LPAREN /* opt_param_list */ RPAREN RETURNS type_specifier block_statement
        { printf("Parser: Function definition for '%s'\n", $2); if($2) free($2); }
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
    printf("Starting parse...\n");
    int result = yyparse();
    if (result == 0) { printf("Parsing completed successfully.\n"); }
    else { printf("Parsing failed (yyparse returned %d).\n", result); }
    if (inputFile != stdin) { fclose(inputFile); }
    return result;
}
