/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "safeguard.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External declarations from the lexer
extern int yylex(void);
extern int current_line;
extern int current_column; // For more precise error reporting
extern char *yytext;     // For reporting the problematic token

// Function to report errors (required by Bison)
void yyerror(const char *s);

// AST related code: Uncomment and define these when you start building the AST.
/*
typedef struct ASTNode {
    char* node_type;
    struct ASTNode *left, *right, *cond, *then_branch, *else_branch; // Example members
    char* s_val;
    int i_val;
    struct ASTNodeList* children; // For lists of things like statements, arguments
    int line;
} ASTNode;

typedef struct ASTNodeList {
    ASTNode* node;
    struct ASTNodeList* next;
} ASTNodeList;

// AST Construction helper function declarations would go here
// e.g., ASTNode* make_binary_op_node(const char* op, ASTNode* left, ASTNode* right, int line);
*/

// You can add %expect 3 here if you want to suppress the 3 S/R warnings once confirmed benign
// %expect 3


#line 110 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 253 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 4,             /* STRING_LITERAL  */
  YYSYMBOL_NUMBER_LITERAL = 5,             /* NUMBER_LITERAL  */
  YYSYMBOL_LET = 6,                        /* LET  */
  YYSYMBOL_NUMBER_TYPE = 7,                /* NUMBER_TYPE  */
  YYSYMBOL_ARRAY_TYPE = 8,                 /* ARRAY_TYPE  */
  YYSYMBOL_TEXT_TYPE = 9,                  /* TEXT_TYPE  */
  YYSYMBOL_OF = 10,                        /* OF  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_LOOP = 12,                      /* LOOP  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_DEFINE = 14,                    /* DEFINE  */
  YYSYMBOL_RETURNS = 15,                   /* RETURNS  */
  YYSYMBOL_RETURN = 16,                    /* RETURN  */
  YYSYMBOL_READ_NUMBER_FN = 17,            /* READ_NUMBER_FN  */
  YYSYMBOL_PRINT_NUMBER_FN = 18,           /* PRINT_NUMBER_FN  */
  YYSYMBOL_PRINT_TEXT_FN = 19,             /* PRINT_TEXT_FN  */
  YYSYMBOL_SIZE_OF = 20,                   /* SIZE_OF  */
  YYSYMBOL_PLUS = 21,                      /* PLUS  */
  YYSYMBOL_MINUS = 22,                     /* MINUS  */
  YYSYMBOL_STAR = 23,                      /* STAR  */
  YYSYMBOL_SLASH = 24,                     /* SLASH  */
  YYSYMBOL_PERCENT = 25,                   /* PERCENT  */
  YYSYMBOL_ASSIGN = 26,                    /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 27,                 /* SEMICOLON  */
  YYSYMBOL_EQUAL_EQUAL = 28,               /* EQUAL_EQUAL  */
  YYSYMBOL_BANG_EQUAL = 29,                /* BANG_EQUAL  */
  YYSYMBOL_LESS = 30,                      /* LESS  */
  YYSYMBOL_LESS_EQUAL = 31,                /* LESS_EQUAL  */
  YYSYMBOL_GREATER = 32,                   /* GREATER  */
  YYSYMBOL_GREATER_EQUAL = 33,             /* GREATER_EQUAL  */
  YYSYMBOL_LPAREN = 34,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 35,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 36,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 37,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 38,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 39,                  /* RBRACKET  */
  YYSYMBOL_COLON = 40,                     /* COLON  */
  YYSYMBOL_ELSE = 41,                      /* ELSE  */
  YYSYMBOL_UMINUS = 42,                    /* UMINUS  */
  YYSYMBOL_43_ = 43,                       /* ','  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_program = 45,                   /* program  */
  YYSYMBOL_declaration_or_function_list = 46, /* declaration_or_function_list  */
  YYSYMBOL_top_level_item = 47,            /* top_level_item  */
  YYSYMBOL_statement_list = 48,            /* statement_list  */
  YYSYMBOL_statement = 49,                 /* statement  */
  YYSYMBOL_block_statement = 50,           /* block_statement  */
  YYSYMBOL_var_declaration = 51,           /* var_declaration  */
  YYSYMBOL_optional_initializer = 52,      /* optional_initializer  */
  YYSYMBOL_type_specifier = 53,            /* type_specifier  */
  YYSYMBOL_expression_statement = 54,      /* expression_statement  */
  YYSYMBOL_if_statement = 55,              /* if_statement  */
  YYSYMBOL_loop_statement = 56,            /* loop_statement  */
  YYSYMBOL_return_statement = 57,          /* return_statement  */
  YYSYMBOL_expression = 58,                /* expression  */
  YYSYMBOL_assignment_expression = 59,     /* assignment_expression  */
  YYSYMBOL_equality_expression = 60,       /* equality_expression  */
  YYSYMBOL_relational_expression = 61,     /* relational_expression  */
  YYSYMBOL_additive_expression = 62,       /* additive_expression  */
  YYSYMBOL_multiplicative_expression = 63, /* multiplicative_expression  */
  YYSYMBOL_unary_expression = 64,          /* unary_expression  */
  YYSYMBOL_postfix_expression = 65,        /* postfix_expression  */
  YYSYMBOL_argument_expression_list_opt = 66, /* argument_expression_list_opt  */
  YYSYMBOL_argument_expression_list = 67,  /* argument_expression_list  */
  YYSYMBOL_primary_expression = 68,        /* primary_expression  */
  YYSYMBOL_lvalue = 69,                    /* lvalue  */
  YYSYMBOL_function_definition = 70        /* function_definition  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   148

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  127

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   297


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    43,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    84,    84,    88,    91,    96,    98,   102,   105,   110,
     111,   112,   113,   114,   115,   116,   121,   126,   131,   134,
     139,   141,   143,   148,   153,   155,   160,   165,   167,   172,
     177,   179,   184,   186,   188,   193,   195,   197,   199,   201,
     206,   208,   210,   215,   217,   219,   221,   226,   228,   233,
     235,   237,   241,   244,   249,   251,   256,   259,   261,   263,
     266,   268,   270,   272,   278,   284,   289
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "STRING_LITERAL", "NUMBER_LITERAL", "LET", "NUMBER_TYPE", "ARRAY_TYPE",
  "TEXT_TYPE", "OF", "IF", "LOOP", "WHILE", "DEFINE", "RETURNS", "RETURN",
  "READ_NUMBER_FN", "PRINT_NUMBER_FN", "PRINT_TEXT_FN", "SIZE_OF", "PLUS",
  "MINUS", "STAR", "SLASH", "PERCENT", "ASSIGN", "SEMICOLON",
  "EQUAL_EQUAL", "BANG_EQUAL", "LESS", "LESS_EQUAL", "GREATER",
  "GREATER_EQUAL", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "COLON", "ELSE", "UMINUS", "','", "$accept", "program",
  "declaration_or_function_list", "top_level_item", "statement_list",
  "statement", "block_statement", "var_declaration",
  "optional_initializer", "type_specifier", "expression_statement",
  "if_statement", "loop_statement", "return_statement", "expression",
  "assignment_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "postfix_expression", "argument_expression_list_opt",
  "argument_expression_list", "primary_expression", "lvalue",
  "function_definition", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-97)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -97,    26,    16,   -97,    36,    41,   -97,   -97,   -97,   -11,
      -3,    54,    11,   -97,    -5,    20,    46,    81,    92,   100,
      96,    84,    54,    67,    80,   -97,   -97,   -97,    86,   -97,
     -97,    87,    96,    96,   -97,   -97,   -23,    35,    30,    79,
     -97,   -26,   -97,   -15,   -97,    89,   112,   -97,    88,   123,
     -97,    91,    93,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,   -97,   -97,
     120,   -97,    97,   -97,    35,    35,    30,    30,    30,    30,
      79,    79,   -97,   -97,   -97,   -97,    98,    94,    95,   -97,
      99,    -2,   -97,   -97,   -97,    96,   -97,   -97,   101,   118,
      90,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     109,   -97,    96,   105,   -97,   113,   -97,   106,    96,   -97,
      37,   107,   102,    37,    37,   -97,   -97
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,     0,     4,     5,     6,     0,
       0,     0,     0,    20,     0,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    58,    57,     0,    61,
      62,     0,     0,     0,    19,    29,    30,    32,    35,    40,
      43,    47,    49,    56,    17,     0,     0,    21,     0,     0,
      48,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     7,    66,
       0,    60,     0,    59,    33,    34,    36,    38,    37,    39,
      41,    42,    44,    45,    46,    54,     0,    53,     0,    31,
       0,     0,    22,    63,    51,     0,    50,    65,     0,     0,
       0,    15,    16,     8,    14,     9,    10,    11,    12,    13,
       0,    55,     0,     0,    28,     0,    23,     0,     0,    27,
       0,     0,    24,     0,     0,    26,    25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -97,   -97,   -97,   -97,   -97,   -96,   103,   142,   -97,   124,
     -97,   -97,   -97,   -97,   -20,   -45,   -97,    25,    19,    10,
     -25,   -97,   -97,   -97,   -97,    28,   -97
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,    91,   103,   104,   105,    21,    16,
     106,   107,   108,   109,   110,    35,    36,    37,    38,    39,
      40,    41,    86,    87,    42,    43,     8
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      34,    25,    26,    27,     4,    53,    54,    50,    64,    98,
      99,    66,    65,    52,   100,    28,    29,    30,    31,    85,
      32,    89,     4,    67,   122,   101,     3,   125,   126,    11,
       5,    12,    33,    18,    68,   102,    82,    83,    84,     9,
      25,    26,    27,     4,    10,    88,    17,    90,    98,    99,
     111,    59,    60,   100,    28,    29,    30,    31,    19,    32,
      51,    13,    14,    15,   101,    55,    56,    57,    58,    80,
      81,    33,    20,    68,    76,    77,    78,    79,    74,    75,
     115,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,   117,    25,    26,    27,    22,    23,   121,    25,
      26,    27,    61,    62,    63,    24,    46,    28,    29,    30,
      31,    44,    32,    28,    29,    30,    31,   114,    32,    47,
      48,    49,    70,    71,    33,    68,    72,    92,    73,    67,
      33,   113,    93,    94,    96,   112,   116,    95,    97,   118,
     119,   120,   123,   124,     7,     0,    45,     0,    69
};

static const yytype_int8 yycheck[] =
{
      20,     3,     4,     5,     6,    28,    29,    32,    34,    11,
      12,    26,    38,    33,    16,    17,    18,    19,    20,    64,
      22,    66,     6,    38,   120,    27,     0,   123,   124,    40,
      14,    34,    34,    38,    36,    37,    61,    62,    63,     3,
       3,     4,     5,     6,     3,    65,    35,    67,    11,    12,
      95,    21,    22,    16,    17,    18,    19,    20,    38,    22,
      32,     7,     8,     9,    27,    30,    31,    32,    33,    59,
      60,    34,    26,    36,    55,    56,    57,    58,    53,    54,
     100,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,   112,     3,     4,     5,    15,     5,   118,     3,
       4,     5,    23,    24,    25,     5,    39,    17,    18,    19,
      20,    27,    22,    17,    18,    19,    20,    27,    22,    39,
      34,    34,    10,    35,    34,    36,     3,     7,    35,    38,
      34,    13,    35,    35,    39,    34,    27,    43,    39,    34,
      27,    35,    35,    41,     2,    -1,    22,    -1,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    45,    46,     0,     6,    14,    47,    51,    70,     3,
       3,    40,    34,     7,     8,     9,    53,    35,    38,    38,
      26,    52,    15,     5,     5,     3,     4,     5,    17,    18,
      19,    20,    22,    34,    58,    59,    60,    61,    62,    63,
      64,    65,    68,    69,    27,    53,    39,    39,    34,    34,
      64,    69,    58,    28,    29,    30,    31,    32,    33,    21,
      22,    23,    24,    25,    34,    38,    26,    38,    36,    50,
      10,    35,     3,    35,    61,    61,    62,    62,    62,    62,
      63,    63,    64,    64,    64,    59,    66,    67,    58,    59,
      58,    48,     7,    35,    35,    43,    39,    39,    11,    12,
      16,    27,    37,    49,    50,    51,    54,    55,    56,    57,
      58,    59,    34,    13,    27,    58,    27,    58,    34,    27,
      35,    58,    49,    35,    41,    49,    49
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    48,    48,    49,
      49,    49,    49,    49,    49,    49,    50,    51,    52,    52,
      53,    53,    53,    54,    55,    55,    56,    57,    57,    58,
      59,    59,    60,    60,    60,    61,    61,    61,    61,    61,
      62,    62,    62,    63,    63,    63,    63,    64,    64,    65,
      65,    65,    66,    66,    67,    67,    68,    68,    68,    68,
      68,    68,    68,    68,    69,    69,    70
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     6,     0,     2,
       1,     4,     6,     2,     5,     7,     6,     3,     2,     1,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     1,
       4,     4,     0,     1,     1,     3,     1,     1,     1,     3,
       3,     1,     1,     4,     1,     4,     7
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: declaration_or_function_list  */
#line 85 "safeguard.y"
    { printf("Parser: Program parsed successfully.\n"); /* $$ = $1; (AST root) */ }
#line 1393 "y.tab.c"
    break;

  case 16: /* block_statement: LBRACE statement_list RBRACE  */
#line 122 "safeguard.y"
        { /* $$ = make_block_node($2, current_line); AST */ }
#line 1399 "y.tab.c"
    break;

  case 17: /* var_declaration: LET IDENTIFIER COLON type_specifier optional_initializer SEMICOLON  */
#line 127 "safeguard.y"
        { printf("Parser: Variable declaration for '%s'\n", (yyvsp[-4].str_val)); if((yyvsp[-4].str_val)) free((yyvsp[-4].str_val)); /* $1 is LET, $2 is ID, etc. */ }
#line 1405 "y.tab.c"
    break;

  case 19: /* optional_initializer: ASSIGN expression  */
#line 135 "safeguard.y"
        { printf("Parser: ...with initializer.\n"); /* $$ = $2; AST */ }
#line 1411 "y.tab.c"
    break;

  case 20: /* type_specifier: NUMBER_TYPE  */
#line 140 "safeguard.y"
        { printf("Parser: Type 'number'.\n"); /* $$ = make_type_node("number", 0, current_line); AST */ }
#line 1417 "y.tab.c"
    break;

  case 21: /* type_specifier: TEXT_TYPE LBRACKET NUMBER_LITERAL RBRACKET  */
#line 142 "safeguard.y"
        { printf("Parser: Type 'text' size %d.\n", (yyvsp[-1].int_val)); /* $$ = make_type_node("text", $3, current_line); AST */ }
#line 1423 "y.tab.c"
    break;

  case 22: /* type_specifier: ARRAY_TYPE LBRACKET NUMBER_LITERAL RBRACKET OF NUMBER_TYPE  */
#line 144 "safeguard.y"
        { printf("Parser: Type 'array' size %d of number.\n", (yyvsp[-3].int_val)); /* $$ = make_array_type_node("number", $3, current_line); AST */ }
#line 1429 "y.tab.c"
    break;

  case 23: /* expression_statement: expression SEMICOLON  */
#line 149 "safeguard.y"
        { /* printf("Parser: Expression statement.\n"); $$ = $1; AST */ }
#line 1435 "y.tab.c"
    break;

  case 24: /* if_statement: IF LPAREN expression RPAREN statement  */
#line 154 "safeguard.y"
        { printf("Parser: If statement (no else).\n"); /* $$ = make_if_node($3, $5, NULL, current_line); AST */ }
#line 1441 "y.tab.c"
    break;

  case 25: /* if_statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 156 "safeguard.y"
        { printf("Parser: If-Else statement.\n"); /* $$ = make_if_node($3, $5, $7, current_line); AST */ }
#line 1447 "y.tab.c"
    break;

  case 26: /* loop_statement: LOOP WHILE LPAREN expression RPAREN statement  */
#line 161 "safeguard.y"
        { printf("Parser: Loop-While statement.\n"); /* $$ = make_loop_node($4, $6, current_line); AST */ }
#line 1453 "y.tab.c"
    break;

  case 27: /* return_statement: RETURN expression SEMICOLON  */
#line 166 "safeguard.y"
        { printf("Parser: Return statement with value.\n"); /* $$ = make_return_node($2, current_line); AST */ }
#line 1459 "y.tab.c"
    break;

  case 28: /* return_statement: RETURN SEMICOLON  */
#line 168 "safeguard.y"
        { printf("Parser: Return statement (void-like).\n"); /* $$ = make_return_node(NULL, current_line); AST */ }
#line 1465 "y.tab.c"
    break;

  case 31: /* assignment_expression: lvalue ASSIGN assignment_expression  */
#line 180 "safeguard.y"
        { printf("Parser: Assignment Expression (e.g. x = ...).\n"); /* $$ = make_assign_node($1, $3, current_line); AST */ }
#line 1471 "y.tab.c"
    break;

  case 33: /* equality_expression: equality_expression EQUAL_EQUAL relational_expression  */
#line 187 "safeguard.y"
        { printf("Parser: Expr ==.\n"); /* $$ = make_binary_op_node("==", $1, $3, current_line); AST */ }
#line 1477 "y.tab.c"
    break;

  case 34: /* equality_expression: equality_expression BANG_EQUAL relational_expression  */
#line 189 "safeguard.y"
        { printf("Parser: Expr !=.\n"); /* $$ = make_binary_op_node("!=", $1, $3, current_line); AST */ }
#line 1483 "y.tab.c"
    break;

  case 36: /* relational_expression: relational_expression LESS additive_expression  */
#line 196 "safeguard.y"
        { printf("Parser: Expr <.\n"); /* $$ = make_binary_op_node("<", $1, $3, current_line); AST */ }
#line 1489 "y.tab.c"
    break;

  case 37: /* relational_expression: relational_expression GREATER additive_expression  */
#line 198 "safeguard.y"
        { printf("Parser: Expr >.\n"); /* $$ = make_binary_op_node(">", $1, $3, current_line); AST */ }
#line 1495 "y.tab.c"
    break;

  case 38: /* relational_expression: relational_expression LESS_EQUAL additive_expression  */
#line 200 "safeguard.y"
        { printf("Parser: Expr <=.\n"); /* $$ = make_binary_op_node("<=", $1, $3, current_line); AST */ }
#line 1501 "y.tab.c"
    break;

  case 39: /* relational_expression: relational_expression GREATER_EQUAL additive_expression  */
#line 202 "safeguard.y"
        { printf("Parser: Expr >=.\n"); /* $$ = make_binary_op_node(">=", $1, $3, current_line); AST */ }
#line 1507 "y.tab.c"
    break;

  case 41: /* additive_expression: additive_expression PLUS multiplicative_expression  */
#line 209 "safeguard.y"
        { printf("Parser: Expr Add.\n"); /* $$ = make_binary_op_node("+", $1, $3, current_line); AST */ }
#line 1513 "y.tab.c"
    break;

  case 42: /* additive_expression: additive_expression MINUS multiplicative_expression  */
#line 211 "safeguard.y"
        { printf("Parser: Expr Subtract.\n"); /* $$ = make_binary_op_node("-", $1, $3, current_line); AST */ }
#line 1519 "y.tab.c"
    break;

  case 44: /* multiplicative_expression: multiplicative_expression STAR unary_expression  */
#line 218 "safeguard.y"
        { printf("Parser: Expr Multiply.\n"); /* $$ = make_binary_op_node("*", $1, $3, current_line); AST */ }
#line 1525 "y.tab.c"
    break;

  case 45: /* multiplicative_expression: multiplicative_expression SLASH unary_expression  */
#line 220 "safeguard.y"
        { printf("Parser: Expr Divide.\n"); /* $$ = make_binary_op_node("/", $1, $3, current_line); AST */ }
#line 1531 "y.tab.c"
    break;

  case 46: /* multiplicative_expression: multiplicative_expression PERCENT unary_expression  */
#line 222 "safeguard.y"
        { printf("Parser: Expr Modulo.\n"); /* $$ = make_binary_op_node("%", $1, $3, current_line); AST */ }
#line 1537 "y.tab.c"
    break;

  case 48: /* unary_expression: MINUS unary_expression  */
#line 229 "safeguard.y"
        { printf("Parser: Expr Unary Minus.\n"); /* $$ = make_unary_op_node("-", $2, current_line); AST */ }
#line 1543 "y.tab.c"
    break;

  case 50: /* postfix_expression: postfix_expression LBRACKET expression RBRACKET  */
#line 236 "safeguard.y"
        { printf("Parser: Array access from postfix 'postfix_expression [ expression ]'.\n"); /* $$ = make_array_access_node($1, $3, current_line); AST */ }
#line 1549 "y.tab.c"
    break;

  case 51: /* postfix_expression: postfix_expression LPAREN argument_expression_list_opt RPAREN  */
#line 238 "safeguard.y"
        { printf("Parser: Function call from postfix 'postfix_expression ( args )'.\n"); /* $$ = make_function_call_node($1, $3, current_line); AST */ }
#line 1555 "y.tab.c"
    break;

  case 57: /* primary_expression: NUMBER_LITERAL  */
#line 260 "safeguard.y"
        { printf("Parser: Primary NUMBER_LITERAL %d\n", (yyvsp[0].int_val)); /* $$ = make_number_node($1, current_line); AST */ }
#line 1561 "y.tab.c"
    break;

  case 58: /* primary_expression: STRING_LITERAL  */
#line 262 "safeguard.y"
        { printf("Parser: Primary STRING_LITERAL \"%s\"\n", (yyvsp[0].str_val)); if((yyvsp[0].str_val)) free((yyvsp[0].str_val)); /* $$ = make_string_node($1, current_line); AST */ }
#line 1567 "y.tab.c"
    break;

  case 59: /* primary_expression: LPAREN expression RPAREN  */
#line 264 "safeguard.y"
        { printf("Parser: Grouped expression.\n"); /* $$ = $2; AST -- pass through the inner expression's AST node */ }
#line 1573 "y.tab.c"
    break;

  case 60: /* primary_expression: READ_NUMBER_FN LPAREN RPAREN  */
#line 267 "safeguard.y"
        { printf("Parser: Primary READ_NUMBER_FN call.\n"); /* $$ = make_builtin_call_node("read_number", NULL, current_line); AST */ }
#line 1579 "y.tab.c"
    break;

  case 61: /* primary_expression: PRINT_NUMBER_FN  */
#line 269 "safeguard.y"
        { printf("Parser: Primary PRINT_NUMBER_FN (as value/callable entity).\n"); /* $$ = make_builtin_name_node("print_number", current_line); AST */ }
#line 1585 "y.tab.c"
    break;

  case 62: /* primary_expression: PRINT_TEXT_FN  */
#line 271 "safeguard.y"
        { printf("Parser: Primary PRINT_TEXT_FN (as value/callable entity).\n"); /* $$ = make_builtin_name_node("print_text", current_line); AST */ }
#line 1591 "y.tab.c"
    break;

  case 63: /* primary_expression: SIZE_OF LPAREN IDENTIFIER RPAREN  */
#line 273 "safeguard.y"
        { printf("Parser: Primary SIZE_OF call for '%s'.\n", (yyvsp[-1].str_val)); if((yyvsp[-1].str_val)) free((yyvsp[-1].str_val)); /* $$ = make_sizeof_node($3, current_line); AST */ }
#line 1597 "y.tab.c"
    break;

  case 65: /* lvalue: lvalue LBRACKET expression RBRACKET  */
#line 285 "safeguard.y"
        { printf("Parser: lvalue -> lvalue [ expr ]\n"); /* $$ = make_array_lval_node($1, $3, current_line); AST */ }
#line 1603 "y.tab.c"
    break;

  case 66: /* function_definition: DEFINE IDENTIFIER LPAREN RPAREN RETURNS type_specifier block_statement  */
#line 290 "safeguard.y"
        { printf("Parser: Function definition for '%s'\n", (yyvsp[-5].str_val)); if((yyvsp[-5].str_val)) free((yyvsp[-5].str_val)); }
#line 1609 "y.tab.c"
    break;


#line 1613 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 296 "safeguard.y"


// C code section

void yyerror(const char *s) {
    fprintf(stderr, "Error (Line %d, near column %d, token '%s'): %s\n",
            current_line, current_column, yytext, s);
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
    extern FILE *yyin;
    yyin = inputFile;

    printf("Starting parse...\n");
    int result = yyparse(); // This calls yylex() internally

    if (result == 0) { // 0 indicates success
        printf("Parsing completed successfully.\n");
    } else { // Non-zero indicates an error (1 for syntax, 2 for memory)
        printf("Parsing failed (yyparse returned %d).\n", result);
    }

    if (inputFile != stdin) {
        fclose(inputFile);
    }
    return result; // Return 0 on success, non-zero on failure
}
