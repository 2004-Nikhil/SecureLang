/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SAFEGUARD_TAB_H_INCLUDED
# define YY_YY_SAFEGUARD_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    STRING_LITERAL = 259,          /* STRING_LITERAL  */
    NUMBER_LITERAL = 260,          /* NUMBER_LITERAL  */
    LET = 261,                     /* LET  */
    IF = 262,                      /* IF  */
    ELSE = 263,                    /* ELSE  */
    LOOP = 264,                    /* LOOP  */
    WHILE = 265,                   /* WHILE  */
    DEFINE = 266,                  /* DEFINE  */
    RETURNS = 267,                 /* RETURNS  */
    RETURN = 268,                  /* RETURN  */
    OF = 269,                      /* OF  */
    SEMICOLON = 270,               /* SEMICOLON  */
    NUMBER_TYPE = 271,             /* NUMBER_TYPE  */
    ARRAY_TYPE = 272,              /* ARRAY_TYPE  */
    TEXT_TYPE = 273,               /* TEXT_TYPE  */
    READ_NUMBER_FN = 274,          /* READ_NUMBER_FN  */
    PRINT_NUMBER_FN = 275,         /* PRINT_NUMBER_FN  */
    PRINT_TEXT_FN = 276,           /* PRINT_TEXT_FN  */
    SIZE_OF = 277,                 /* SIZE_OF  */
    PLUS = 278,                    /* PLUS  */
    MINUS = 279,                   /* MINUS  */
    STAR = 280,                    /* STAR  */
    SLASH = 281,                   /* SLASH  */
    PERCENT = 282,                 /* PERCENT  */
    ASSIGN = 283,                  /* ASSIGN  */
    EQUAL_EQUAL = 284,             /* EQUAL_EQUAL  */
    BANG_EQUAL = 285,              /* BANG_EQUAL  */
    LESS = 286,                    /* LESS  */
    LESS_EQUAL = 287,              /* LESS_EQUAL  */
    GREATER = 288,                 /* GREATER  */
    GREATER_EQUAL = 289,           /* GREATER_EQUAL  */
    LPAREN = 290,                  /* LPAREN  */
    RPAREN = 291,                  /* RPAREN  */
    LBRACE = 292,                  /* LBRACE  */
    RBRACE = 293,                  /* RBRACE  */
    LBRACKET = 294,                /* LBRACKET  */
    RBRACKET = 295,                /* RBRACKET  */
    COLON = 296,                   /* COLON  */
    UMINUS = 297                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "safeguard.y"

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

#line 129 "safeguard.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SAFEGUARD_TAB_H_INCLUDED  */
