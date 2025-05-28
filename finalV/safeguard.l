%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "safeguard.tab.h" // For yylval union definition and token codes
#include "ast.h"           // For unescape_string prototype

// Flex's yylineno will be our source of truth for the current line


char* unescape_string(const char* s, int len);
// No need for global current_column here if we're only focusing on line numbers for AST
%}
%option yylineno
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID_CHAR     [a-zA-Z0-9_]

%%

"let"           { yylval.token_loc.line = yylineno; return LET; }
"number"        { yylval.token_loc.line = yylineno; return NUMBER_TYPE; }
"array"         { yylval.token_loc.line = yylineno; return ARRAY_TYPE; }
"text"          { yylval.token_loc.line = yylineno; return TEXT_TYPE; }
"of"            { yylval.token_loc.line = yylineno; return OF; }
"if"            { yylval.token_loc.line = yylineno; return IF; }
"else"          { yylval.token_loc.line = yylineno; return ELSE; }
"loop"          { yylval.token_loc.line = yylineno; return LOOP; }
"while"         { yylval.token_loc.line = yylineno; return WHILE; }
"define"        { yylval.token_loc.line = yylineno; return DEFINE; }
"returns"       { yylval.token_loc.line = yylineno; return RETURNS; }
"return"        { yylval.token_loc.line = yylineno; return RETURN; }

"read_number"   { yylval.token_loc.line = yylineno; return READ_NUMBER_FN; }
"print_number"  { yylval.token_loc.line = yylineno; return PRINT_NUMBER_FN; }
"print_text"    { yylval.token_loc.line = yylineno; return PRINT_TEXT_FN; }
"size_of"       { yylval.token_loc.line = yylineno; return SIZE_OF; }


{LETTER}{ID_CHAR}* {
    yylval.token_str_loc.s_val = strdup(yytext);
    yylval.token_str_loc.line = yylineno;
    return IDENTIFIER;
}

"-"?{DIGIT}+ {
    yylval.token_int_loc.i_val = atoi(yytext);
    yylval.token_int_loc.line = yylineno;
    return NUMBER_LITERAL;
}

"~".*           { /* Comment, no token */ }

\"(\\.|[^\\"\n])*\" {
    char* content_buffer = (char*)malloc(yyleng - 1);
    if (!content_buffer) { perror("malloc in string lexer rule"); exit(EXIT_FAILURE); }
    strncpy(content_buffer, yytext + 1, yyleng - 2);
    content_buffer[yyleng - 2] = '\0';
    yylval.token_str_loc.s_val = unescape_string(content_buffer, strlen(content_buffer));
    yylval.token_str_loc.line = yylineno;
    free(content_buffer);
    return STRING_LITERAL;
}

\"(\\.|[^\\"\n])*\n      { fprintf(stderr, "Lexer Error (Line %d): Unterminated string literal.\n", yylineno); /* yylineno already advanced by Flex for the \n */ }


"+"             { yylval.token_loc.line = yylineno; return PLUS; }
"-"             { yylval.token_loc.line = yylineno; return MINUS; }
"*"             { yylval.token_loc.line = yylineno; return STAR; }
"/"             { yylval.token_loc.line = yylineno; return SLASH; }
"%"             { yylval.token_loc.line = yylineno; return PERCENT; }
"="             { yylval.token_loc.line = yylineno; return ASSIGN; }
"=="            { yylval.token_loc.line = yylineno; return EQUAL_EQUAL; }
"!="            { yylval.token_loc.line = yylineno; return BANG_EQUAL; }
"<"             { yylval.token_loc.line = yylineno; return LESS; }
"<="            { yylval.token_loc.line = yylineno; return LESS_EQUAL; }
">"             { yylval.token_loc.line = yylineno; return GREATER; }
">="            { yylval.token_loc.line = yylineno; return GREATER_EQUAL; }
"("             { yylval.token_loc.line = yylineno; return LPAREN; }
")"             { yylval.token_loc.line = yylineno; return RPAREN; }
"{"             { yylval.token_loc.line = yylineno; return LBRACE; }
"}"             { yylval.token_loc.line = yylineno; return RBRACE; }
"["             { yylval.token_loc.line = yylineno; return LBRACKET; }
"]"             { yylval.token_loc.line = yylineno; return RBRACKET; }
":"             { yylval.token_loc.line = yylineno; return COLON; }
";"             { yylval.token_loc.line = yylineno; return SEMICOLON; }


[ \t]+          { /* Skip whitespace */ }
\n              { /* Flex handles yylineno automatically */ }

.               { fprintf(stderr, "Lexer Error (Line %d): Unknown character '%c'\n", yylineno, yytext[0]); }

%%
// int yywrap(void) { return 1; } // Needed if not using noyywrap
