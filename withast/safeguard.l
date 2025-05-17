%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "safeguard.tab.h" // Bison-generated header (contains YYSTYPE and token defs)
#include "ast.h" // Not strictly needed in lexer, but good for consistency

// YYSTYPE (yylval) is defined by Bison in safeguard.tab.h
// extern YYSTYPE yylval;

// Global variables for line and column tracking - used by parser for AST nodes
int current_line = 1;
int current_column = 1; // Column tracking can be basic for now

// Forward declaration for Bison's error reporting function if needed by lexer directly
// extern void yyerror(const char* s);

// unescape_string function (ensure it's defined, perhaps in ast.c or a utils.c)
char* unescape_string(const char* s, int len); // Assuming it's in ast.c or linked

%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID_CHAR     [a-zA-Z0-9_]

%%

"let"           { current_column += yyleng; return LET; }
"number"        { current_column += yyleng; return NUMBER_TYPE; }
"array"         { current_column += yyleng; return ARRAY_TYPE; }
"text"          { current_column += yyleng; return TEXT_TYPE; }
"of"            { current_column += yyleng; return OF; }
"if"            { current_column += yyleng; return IF; }
"else"          { current_column += yyleng; return ELSE; }
"loop"          { current_column += yyleng; return LOOP; }
"while"         { current_column += yyleng; return WHILE; }
"define"        { current_column += yyleng; return DEFINE; }
"returns"       { current_column += yyleng; return RETURNS; }
"return"        { current_column += yyleng; return RETURN; }

"read_number"   { current_column += yyleng; return READ_NUMBER_FN; }
"print_number"  { current_column += yyleng; return PRINT_NUMBER_FN; }
"print_text"    { current_column += yyleng; return PRINT_TEXT_FN; }
"size_of"       { current_column += yyleng; return SIZE_OF; }


{LETTER}{ID_CHAR}* {
    yylval.str_val = strdup(yytext); // AST node will own this
    current_column += yyleng;
    return IDENTIFIER;
}

"-"?{DIGIT}+ {
    yylval.int_val = atoi(yytext);
    current_column += yyleng;
    return NUMBER_LITERAL;
}

"~".*           { /* Comment: No action, column will be reset by newline or updated by next token */ }

\"(\\.|[^\\"\n])*\" { // String literal
    char* content_buffer = (char*)malloc(yyleng - 1); // For content without quotes
    if (!content_buffer) { perror("malloc in string lexer rule"); exit(EXIT_FAILURE); }
    strncpy(content_buffer, yytext + 1, yyleng - 2);
    content_buffer[yyleng - 2] = '\0';
    yylval.str_val = unescape_string(content_buffer, strlen(content_buffer)); // AST node will own this
    free(content_buffer);
    current_column += yyleng;
    return STRING_LITERAL;
}

\"(\\.|[^\\"\n])*\n { // Unterminated string
    fprintf(stderr, "Lexer Error (Line %d, Col %d): Unterminated string literal.\n", current_line, current_column);
    // current_column is approx start of unterminated string here before advancing for newline
    current_line++;
    current_column = 1;
    // Skip returning a token, or return a specific error token if parser handles it
}


"+"             { current_column += yyleng; return PLUS; }
"-"             { current_column += yyleng; return MINUS; }
"*"             { current_column += yyleng; return STAR; }
"/"             { current_column += yyleng; return SLASH; }
"%"             { current_column += yyleng; return PERCENT; }
"="             { current_column += yyleng; return ASSIGN; }
"=="            { current_column += yyleng; return EQUAL_EQUAL; }
"!="            { current_column += yyleng; return BANG_EQUAL; }
"<"             { current_column += yyleng; return LESS; }
"<="            { current_column += yyleng; return LESS_EQUAL; }
">"             { current_column += yyleng; return GREATER; }
">="            { current_column += yyleng; return GREATER_EQUAL; }
"("             { current_column += yyleng; return LPAREN; }
")"             { current_column += yyleng; return RPAREN; }
"{"             { current_column += yyleng; return LBRACE; }
"}"             { current_column += yyleng; return RBRACE; }
"["             { current_column += yyleng; return LBRACKET; }
"]"             { current_column += yyleng; return RBRACKET; }
":"             { current_column += yyleng; return COLON; }
";"             { current_column += yyleng; return SEMICOLON; }


[ \t]+          { current_column += yyleng; /* Skip whitespace */ }
\n              { current_line++; current_column = 1; /* Reset for newline */ }

.               {
    fprintf(stderr, "Lexer Error (Line %d, Col %d): Unknown character '%c'\n", current_line, current_column, yytext[0]);
    current_column += yyleng; // Advance past unknown char
    // Optionally return an UNKNOWN token if the parser has a recovery rule for it
}

%%
int yywrap(void) {
    return 1; // Signal end of input
}

// The main function is now in safeguard.y
