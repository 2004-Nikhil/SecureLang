%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "safeguard.tab.h" // Use the correct generated header name

// extern YYSTYPE yylval; // Declared by Bison

int current_line = 1;
int current_column = 1;

void update_column(int length) {
    current_column += length;
}

char* unescape_string(const char* s, int len) {
    // ... (unescape_string function as before) ...
    char* result = (char*)malloc(len + 1);
    if (!result) { perror("malloc failed in unescape_string"); exit(EXIT_FAILURE); }
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (s[i] == '\\' && i + 1 < len) {
            i++;
            switch (s[i]) {
                case 'n': result[j++] = '\n'; break;
                case 't': result[j++] = '\t'; break;
                case '\\': result[j++] = '\\'; break;
                case '"': result[j++] = '"'; break;
                default: result[j++] = '\\'; result[j++] = s[i];
                    fprintf(stderr, "Warning (Line %d): Invalid escape sequence '\\%c'\n", current_line, s[i]); break;
            }
        } else { result[j++] = s[i]; }
    }
    result[j] = '\0';
    return result;
}
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID_CHAR     [a-zA-Z0-9_]

%%

"let"           { update_column(yyleng); return LET; }
"number"        { update_column(yyleng); return NUMBER_TYPE; }
"array"         { update_column(yyleng); return ARRAY_TYPE; }
"text"          { update_column(yyleng); return TEXT_TYPE; }
"of"            { update_column(yyleng); return OF; }
"if"            { update_column(yyleng); return IF; }
"else"          { update_column(yyleng); return ELSE; }
"loop"          { update_column(yyleng); return LOOP; }
"while"         { update_column(yyleng); return WHILE; }
"define"        { update_column(yyleng); return DEFINE; }
"returns"       { update_column(yyleng); return RETURNS; }
"return"        { update_column(yyleng); return RETURN; }
"read_number"   { update_column(yyleng); return READ_NUMBER_FN; }
"print_number"  { update_column(yyleng); return PRINT_NUMBER_FN; }
"print_text"    { update_column(yyleng); return PRINT_TEXT_FN; }
"size_of"       { update_column(yyleng); return SIZE_OF; }

{LETTER}{ID_CHAR}* { yylval.str_val = strdup(yytext); update_column(yyleng); return IDENTIFIER; }
"-"?{DIGIT}+      { yylval.int_val = atoi(yytext); update_column(yyleng); return NUMBER_LITERAL; }

"~".*           { /* Comment */ }

\"(\\.|[^\\"\n])*\" {
    char* content_buffer = (char*)malloc(yyleng - 1);
    if (!content_buffer) { perror("malloc in string rule"); exit(EXIT_FAILURE); }
    strncpy(content_buffer, yytext + 1, yyleng - 2);
    content_buffer[yyleng - 2] = '\0';
    yylval.str_val = unescape_string(content_buffer, strlen(content_buffer));
    free(content_buffer);
    update_column(yyleng);
    return STRING_LITERAL;
}
\"(\\.|[^\\"\n])*\n {
    fprintf(stderr, "Error (Line %d, Col %d): Unterminated string literal.\n", current_line, current_column - (yyleng -1) );
    current_line++; current_column = 1;
}

"+"             { update_column(yyleng); return PLUS; }
"-"             { update_column(yyleng); return MINUS; }
"*"             { update_column(yyleng); return STAR; }
"/"             { update_column(yyleng); return SLASH; }
"%"             { update_column(yyleng); return PERCENT; }
"="             { update_column(yyleng); return ASSIGN; }
"=="            { update_column(yyleng); return EQUAL_EQUAL; }
"!="            { update_column(yyleng); return BANG_EQUAL; }
"<"             { update_column(yyleng); return LESS; }
"<="            { update_column(yyleng); return LESS_EQUAL; }
">"             { update_column(yyleng); return GREATER; }
">="            { update_column(yyleng); return GREATER_EQUAL; }
"("             { update_column(yyleng); return LPAREN; }
")"             { update_column(yyleng); return RPAREN; }
"{"             { update_column(yyleng); return LBRACE; }
"}"             { update_column(yyleng); return RBRACE; }
"["             { update_column(yyleng); return LBRACKET; }
"]"             { update_column(yyleng); return RBRACKET; }
":"             { update_column(yyleng); return COLON; }
";"             { update_column(yyleng); return SEMICOLON; } // <-- ADDED SEMICOLON RULE

[ \t]+          { update_column(yyleng); }
\n              { current_line++; current_column = 1; }
.               { fprintf(stderr, "Lexer Error (Line %d, Col %d): Unknown character '%c'\n", current_line, current_column, yytext[0]); update_column(yyleng); /* No return, lexer skips */ }

%%
int yywrap(void) { return 1; }
