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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    NUMBER_TYPE = 262,             /* NUMBER_TYPE  */
    ARRAY_TYPE = 263,              /* ARRAY_TYPE  */
    TEXT_TYPE = 264,               /* TEXT_TYPE  */
    OF = 265,                      /* OF  */
    IF = 266,                      /* IF  */
    LOOP = 267,                    /* LOOP  */
    WHILE = 268,                   /* WHILE  */
    DEFINE = 269,                  /* DEFINE  */
    RETURNS = 270,                 /* RETURNS  */
    RETURN = 271,                  /* RETURN  */
    READ_NUMBER_FN = 272,          /* READ_NUMBER_FN  */
    PRINT_NUMBER_FN = 273,         /* PRINT_NUMBER_FN  */
    PRINT_TEXT_FN = 274,           /* PRINT_TEXT_FN  */
    SIZE_OF = 275,                 /* SIZE_OF  */
    PLUS = 276,                    /* PLUS  */
    MINUS = 277,                   /* MINUS  */
    STAR = 278,                    /* STAR  */
    SLASH = 279,                   /* SLASH  */
    PERCENT = 280,                 /* PERCENT  */
    ASSIGN = 281,                  /* ASSIGN  */
    SEMICOLON = 282,               /* SEMICOLON  */
    EQUAL_EQUAL = 283,             /* EQUAL_EQUAL  */
    BANG_EQUAL = 284,              /* BANG_EQUAL  */
    LESS = 285,                    /* LESS  */
    LESS_EQUAL = 286,              /* LESS_EQUAL  */
    GREATER = 287,                 /* GREATER  */
    GREATER_EQUAL = 288,           /* GREATER_EQUAL  */
    LPAREN = 289,                  /* LPAREN  */
    RPAREN = 290,                  /* RPAREN  */
    LBRACE = 291,                  /* LBRACE  */
    RBRACE = 292,                  /* RBRACE  */
    LBRACKET = 293,                /* LBRACKET  */
    RBRACKET = 294,                /* RBRACKET  */
    COLON = 295,                   /* COLON  */
    ELSE = 296,                    /* ELSE  */
    UMINUS = 297                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENTIFIER 258
#define STRING_LITERAL 259
#define NUMBER_LITERAL 260
#define LET 261
#define NUMBER_TYPE 262
#define ARRAY_TYPE 263
#define TEXT_TYPE 264
#define OF 265
#define IF 266
#define LOOP 267
#define WHILE 268
#define DEFINE 269
#define RETURNS 270
#define RETURN 271
#define READ_NUMBER_FN 272
#define PRINT_NUMBER_FN 273
#define PRINT_TEXT_FN 274
#define SIZE_OF 275
#define PLUS 276
#define MINUS 277
#define STAR 278
#define SLASH 279
#define PERCENT 280
#define ASSIGN 281
#define SEMICOLON 282
#define EQUAL_EQUAL 283
#define BANG_EQUAL 284
#define LESS 285
#define LESS_EQUAL 286
#define GREATER 287
#define GREATER_EQUAL 288
#define LPAREN 289
#define RPAREN 290
#define LBRACE 291
#define RBRACE 292
#define LBRACKET 293
#define RBRACKET 294
#define COLON 295
#define ELSE 296
#define UMINUS 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 41 "safeguard.y"

    int int_val;        // For NUMBER_LITERAL
    char *str_val;      // For IDENTIFIER, STRING_LITERAL
    // struct ASTNode *node_ptr; // For AST nodes (when building the AST)

#line 157 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
