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

#line 90 "safeguard.tab.c"

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

#include "safeguard.tab.h"
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
  YYSYMBOL_item_list = 46,                 /* item_list  */
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
typedef yytype_uint8 yy_state_t;

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   173

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  133

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
static const yytype_uint8 yyrline[] =
{
       0,    60,    60,    66,    67,    71,    72,    76,    77,    81,
      82,    83,    84,    85,    86,    87,    91,    96,   101,   102,
     106,   108,   110,   115,   120,   122,   127,   132,   134,   139,
     143,   144,   149,   150,   152,   157,   158,   160,   162,   164,
     169,   170,   172,   177,   178,   180,   182,   187,   188,   193,
     194,   196,   198,   201,   207,   208,   212,   213,   217,   218,
     220,   222,   224,   226,   231,   233,   238
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
  "item_list", "top_level_item", "statement_list", "statement",
  "block_statement", "var_declaration", "optional_initializer",
  "type_specifier", "expression_statement", "if_statement",
  "loop_statement", "return_statement", "expression",
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

#define YYPACT_NINF (-54)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -54,     8,    93,   -54,   -54,   -54,   -54,     7,   -21,     3,
      25,     0,    31,    45,    50,    54,   139,   -54,   139,   -54,
     -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,    62,   -54,
      30,    -7,    46,    14,   -54,   -23,   -54,   -17,   -54,    51,
     139,    56,    59,   -54,    73,    66,   139,   139,    99,   -54,
      68,    79,    58,   -54,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,    74,
      81,   139,    82,   -54,   -54,    83,    84,    86,   -54,   -54,
     -54,    -7,    -7,    46,    46,    46,    46,    14,    14,   -54,
     -54,   -54,   -54,    91,    65,    89,   -54,    94,   -54,    96,
     102,   106,   119,   110,   132,   -54,   -54,   -54,   -54,   139,
     -54,   -54,   143,   144,   139,   123,   111,   119,    74,   -54,
     112,   115,   -54,   -54,   119,   -54,   124,   152,   -54,   -54,
     -54,   156,   -54
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,    64,    60,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,     0,     7,
       4,     6,    14,     9,    10,    11,    12,    13,     0,    29,
      30,    32,    35,    40,    43,    47,    49,    58,     5,     0,
       0,     0,     0,    28,     0,     0,     0,     0,     0,    48,
      58,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,    27,    62,     0,     0,     0,    61,    16,
       8,    33,    34,    36,    38,    37,    39,    41,    42,    44,
      45,    46,    56,     0,    55,     0,    31,     0,    20,     0,
       0,    18,     0,     0,     0,    52,    53,    63,    51,     0,
      50,    65,     0,     0,     0,     0,    24,     0,     0,    57,
       0,     0,    19,    17,     0,    26,     0,     0,    21,    25,
      66,     0,    22
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -54,   -54,   -54,   -54,   -54,   -51,    38,   -54,   -54,    47,
     -54,   -54,   -54,   -54,   -11,   -53,   -54,    17,   -26,    26,
     -10,   -54,   -54,   -54,   -54,   -14,   -54
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    20,    52,    21,    22,    23,   115,   101,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    93,    94,    36,    37,    38
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      44,    80,    50,     4,     5,     6,    49,    51,     3,    67,
      39,    65,    92,    40,    96,    66,    41,    12,    13,    14,
      15,    68,    16,    56,    57,    58,    59,    43,    42,    70,
      83,    84,    85,    86,    18,    75,    76,    62,    63,    64,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,   116,    89,    90,    91,    95,   119,    97,    54,    55,
     103,     4,     5,     6,     7,    45,   125,    60,    61,     8,
       9,    81,    82,   129,    11,    12,    13,    14,    15,    46,
      16,    98,    99,   100,    47,    17,    87,    88,    48,    53,
      71,    69,    18,    72,    19,    79,     4,     5,     6,     7,
      73,    74,    77,   122,     8,     9,    68,    10,   109,    11,
      12,    13,    14,    15,    78,    16,   102,   104,   105,   106,
      17,   107,     4,     5,     6,     7,   108,    18,   110,    19,
       8,     9,   114,   111,   112,    11,    12,    13,    14,    15,
     113,    16,     4,     5,     6,   117,    17,   118,   120,   121,
     123,   127,   124,    18,   128,    19,    12,    13,    14,    15,
      19,    16,   131,   132,   130,   126,     0,     0,     0,     0,
       0,     0,     0,    18
};

static const yytype_int8 yycheck[] =
{
      11,    52,    16,     3,     4,     5,    16,    18,     0,    26,
       3,    34,    65,    34,    67,    38,    13,    17,    18,    19,
      20,    38,    22,    30,    31,    32,    33,    27,     3,    40,
      56,    57,    58,    59,    34,    46,    47,    23,    24,    25,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,   102,    62,    63,    64,    66,   109,    68,    28,    29,
      71,     3,     4,     5,     6,    34,   117,    21,    22,    11,
      12,    54,    55,   124,    16,    17,    18,    19,    20,    34,
      22,     7,     8,     9,    34,    27,    60,    61,    34,    27,
      34,    40,    34,    34,    36,    37,     3,     4,     5,     6,
      27,    35,     3,   114,    11,    12,    38,    14,    43,    16,
      17,    18,    19,    20,    35,    22,    35,    35,    35,    35,
      27,    35,     3,     4,     5,     6,    35,    34,    39,    36,
      11,    12,    26,    39,    38,    16,    17,    18,    19,    20,
      38,    22,     3,     4,     5,    35,    27,    15,     5,     5,
      27,    39,    41,    34,    39,    36,    17,    18,    19,    20,
      36,    22,    10,     7,   126,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    45,    46,     0,     3,     4,     5,     6,    11,    12,
      14,    16,    17,    18,    19,    20,    22,    27,    34,    36,
      47,    49,    50,    51,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    68,    69,    70,     3,
      34,    13,     3,    27,    58,    34,    34,    34,    34,    64,
      69,    58,    48,    27,    28,    29,    30,    31,    32,    33,
      21,    22,    23,    24,    25,    34,    38,    26,    38,    40,
      58,    34,    34,    27,    35,    58,    58,     3,    35,    37,
      49,    61,    61,    62,    62,    62,    62,    63,    63,    64,
      64,    64,    59,    66,    67,    58,    59,    58,     7,     8,
       9,    53,    35,    58,    35,    35,    35,    35,    35,    43,
      39,    39,    38,    38,    26,    52,    49,    35,    15,    59,
       5,     5,    58,    27,    41,    49,    53,    39,    39,    49,
      50,    10,     7
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    48,    48,    49,
      49,    49,    49,    49,    49,    49,    50,    51,    52,    52,
      53,    53,    53,    54,    55,    55,    56,    57,    57,    58,
      59,    59,    60,    60,    60,    61,    61,    61,    61,    61,
      62,    62,    62,    63,    63,    63,    63,    64,    64,    65,
      65,    65,    65,    65,    66,    66,    67,    67,    68,    68,
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
       4,     4,     4,     4,     0,     1,     1,     3,     1,     1,
       1,     3,     3,     4,     1,     4,     7
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: item_list  */
#line 62 "safeguard.y"
    { (yyval.node_ptr) = create_program_node((yyvsp[0].node_list_ptr)); ast_root = (yyval.node_ptr); }
#line 1363 "safeguard.tab.c"
    break;

  case 3: /* item_list: %empty  */
#line 66 "safeguard.y"
                                { (yyval.node_list_ptr) = NULL; }
#line 1369 "safeguard.tab.c"
    break;

  case 4: /* item_list: item_list top_level_item  */
#line 67 "safeguard.y"
                                { (yyval.node_list_ptr) = append_to_list((yyvsp[-1].node_list_ptr), (yyvsp[0].node_ptr)); }
#line 1375 "safeguard.tab.c"
    break;

  case 5: /* top_level_item: function_definition  */
#line 71 "safeguard.y"
                        { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1381 "safeguard.tab.c"
    break;

  case 6: /* top_level_item: statement  */
#line 72 "safeguard.y"
                          { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1387 "safeguard.tab.c"
    break;

  case 7: /* statement_list: %empty  */
#line 76 "safeguard.y"
                                { (yyval.node_list_ptr) = NULL; }
#line 1393 "safeguard.tab.c"
    break;

  case 8: /* statement_list: statement_list statement  */
#line 77 "safeguard.y"
                                { (yyval.node_list_ptr) = append_to_list((yyvsp[-1].node_list_ptr), (yyvsp[0].node_ptr)); }
#line 1399 "safeguard.tab.c"
    break;

  case 9: /* statement: var_declaration  */
#line 81 "safeguard.y"
                            { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1405 "safeguard.tab.c"
    break;

  case 10: /* statement: expression_statement  */
#line 82 "safeguard.y"
                            { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1411 "safeguard.tab.c"
    break;

  case 11: /* statement: if_statement  */
#line 83 "safeguard.y"
                            { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1417 "safeguard.tab.c"
    break;

  case 12: /* statement: loop_statement  */
#line 84 "safeguard.y"
                            { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1423 "safeguard.tab.c"
    break;

  case 13: /* statement: return_statement  */
#line 85 "safeguard.y"
                            { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1429 "safeguard.tab.c"
    break;

  case 14: /* statement: block_statement  */
#line 86 "safeguard.y"
                            { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1435 "safeguard.tab.c"
    break;

  case 15: /* statement: SEMICOLON  */
#line 87 "safeguard.y"
                            { (yyval.node_ptr) = create_empty_stmt_node((yylsp[0]).first_line); }
#line 1441 "safeguard.tab.c"
    break;

  case 16: /* block_statement: LBRACE statement_list RBRACE  */
#line 92 "safeguard.y"
    { (yyval.node_ptr) = create_block_stmt_node((yyvsp[-1].node_list_ptr), (yylsp[-2]).first_line); }
#line 1447 "safeguard.tab.c"
    break;

  case 17: /* var_declaration: LET IDENTIFIER COLON type_specifier optional_initializer SEMICOLON  */
#line 97 "safeguard.y"
    { (yyval.node_ptr) = create_var_decl_node((yyvsp[-4].str_val), (yyvsp[-2].node_ptr), (yyvsp[-1].node_ptr), (yylsp[-5]).first_line); }
#line 1453 "safeguard.tab.c"
    break;

  case 18: /* optional_initializer: %empty  */
#line 101 "safeguard.y"
                        { (yyval.node_ptr) = NULL; }
#line 1459 "safeguard.tab.c"
    break;

  case 19: /* optional_initializer: ASSIGN expression  */
#line 102 "safeguard.y"
                        { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1465 "safeguard.tab.c"
    break;

  case 20: /* type_specifier: NUMBER_TYPE  */
#line 107 "safeguard.y"
    { (yyval.node_ptr) = create_type_spec_node("number", 0, (yylsp[0]).first_line); }
#line 1471 "safeguard.tab.c"
    break;

  case 21: /* type_specifier: TEXT_TYPE LBRACKET NUMBER_LITERAL RBRACKET  */
#line 109 "safeguard.y"
    { (yyval.node_ptr) = create_type_spec_node("text", (yyvsp[-1].int_val), (yylsp[-3]).first_line); }
#line 1477 "safeguard.tab.c"
    break;

  case 22: /* type_specifier: ARRAY_TYPE LBRACKET NUMBER_LITERAL RBRACKET OF NUMBER_TYPE  */
#line 111 "safeguard.y"
    { (yyval.node_ptr) = create_type_spec_node("array", (yyvsp[-3].int_val), (yylsp[-5]).first_line); }
#line 1483 "safeguard.tab.c"
    break;

  case 23: /* expression_statement: expression SEMICOLON  */
#line 116 "safeguard.y"
    { (yyval.node_ptr) = create_expression_stmt_node((yyvsp[-1].node_ptr), (yyvsp[-1].node_ptr)->line_number); }
#line 1489 "safeguard.tab.c"
    break;

  case 24: /* if_statement: IF LPAREN expression RPAREN statement  */
#line 121 "safeguard.y"
    { (yyval.node_ptr) = create_if_stmt_node((yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), NULL, (yylsp[-4]).first_line); }
#line 1495 "safeguard.tab.c"
    break;

  case 25: /* if_statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 123 "safeguard.y"
    { (yyval.node_ptr) = create_if_stmt_node((yyvsp[-4].node_ptr), (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-6]).first_line); }
#line 1501 "safeguard.tab.c"
    break;

  case 26: /* loop_statement: LOOP WHILE LPAREN expression RPAREN statement  */
#line 128 "safeguard.y"
    { (yyval.node_ptr) = create_loop_stmt_node((yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-5]).first_line); }
#line 1507 "safeguard.tab.c"
    break;

  case 27: /* return_statement: RETURN expression SEMICOLON  */
#line 133 "safeguard.y"
    { (yyval.node_ptr) = create_return_stmt_node((yyvsp[-1].node_ptr), (yylsp[-2]).first_line); }
#line 1513 "safeguard.tab.c"
    break;

  case 28: /* return_statement: RETURN SEMICOLON  */
#line 135 "safeguard.y"
    { (yyval.node_ptr) = create_return_stmt_node(NULL, (yylsp[-1]).first_line); }
#line 1519 "safeguard.tab.c"
    break;

  case 29: /* expression: assignment_expression  */
#line 139 "safeguard.y"
                          { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1525 "safeguard.tab.c"
    break;

  case 30: /* assignment_expression: equality_expression  */
#line 143 "safeguard.y"
                        { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1531 "safeguard.tab.c"
    break;

  case 31: /* assignment_expression: lvalue ASSIGN assignment_expression  */
#line 145 "safeguard.y"
    { (yyval.node_ptr) = create_assignment_expr_node((yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1537 "safeguard.tab.c"
    break;

  case 32: /* equality_expression: relational_expression  */
#line 149 "safeguard.y"
                          { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1543 "safeguard.tab.c"
    break;

  case 33: /* equality_expression: equality_expression EQUAL_EQUAL relational_expression  */
#line 151 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("==", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1549 "safeguard.tab.c"
    break;

  case 34: /* equality_expression: equality_expression BANG_EQUAL relational_expression  */
#line 153 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("!=", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1555 "safeguard.tab.c"
    break;

  case 35: /* relational_expression: additive_expression  */
#line 157 "safeguard.y"
                        { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1561 "safeguard.tab.c"
    break;

  case 36: /* relational_expression: relational_expression LESS additive_expression  */
#line 159 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("<", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1567 "safeguard.tab.c"
    break;

  case 37: /* relational_expression: relational_expression GREATER additive_expression  */
#line 161 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node(">", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1573 "safeguard.tab.c"
    break;

  case 38: /* relational_expression: relational_expression LESS_EQUAL additive_expression  */
#line 163 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("<=", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1579 "safeguard.tab.c"
    break;

  case 39: /* relational_expression: relational_expression GREATER_EQUAL additive_expression  */
#line 165 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node(">=", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1585 "safeguard.tab.c"
    break;

  case 40: /* additive_expression: multiplicative_expression  */
#line 169 "safeguard.y"
                              { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1591 "safeguard.tab.c"
    break;

  case 41: /* additive_expression: additive_expression PLUS multiplicative_expression  */
#line 171 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("+", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1597 "safeguard.tab.c"
    break;

  case 42: /* additive_expression: additive_expression MINUS multiplicative_expression  */
#line 173 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("-", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1603 "safeguard.tab.c"
    break;

  case 43: /* multiplicative_expression: unary_expression  */
#line 177 "safeguard.y"
                     { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1609 "safeguard.tab.c"
    break;

  case 44: /* multiplicative_expression: multiplicative_expression STAR unary_expression  */
#line 179 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("*", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1615 "safeguard.tab.c"
    break;

  case 45: /* multiplicative_expression: multiplicative_expression SLASH unary_expression  */
#line 181 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("/", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1621 "safeguard.tab.c"
    break;

  case 46: /* multiplicative_expression: multiplicative_expression PERCENT unary_expression  */
#line 183 "safeguard.y"
    { (yyval.node_ptr) = create_binary_op_node("%", (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1627 "safeguard.tab.c"
    break;

  case 47: /* unary_expression: postfix_expression  */
#line 187 "safeguard.y"
                       { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1633 "safeguard.tab.c"
    break;

  case 48: /* unary_expression: MINUS unary_expression  */
#line 189 "safeguard.y"
    { (yyval.node_ptr) = create_unary_op_node("-", (yyvsp[0].node_ptr), (yylsp[-1]).first_line); }
#line 1639 "safeguard.tab.c"
    break;

  case 49: /* postfix_expression: primary_expression  */
#line 193 "safeguard.y"
                       { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1645 "safeguard.tab.c"
    break;

  case 50: /* postfix_expression: postfix_expression LBRACKET expression RBRACKET  */
#line 195 "safeguard.y"
    { (yyval.node_ptr) = create_array_access_node((yyvsp[-3].node_ptr), (yyvsp[-1].node_ptr), (yyvsp[-3].node_ptr)->line_number); }
#line 1651 "safeguard.tab.c"
    break;

  case 51: /* postfix_expression: postfix_expression LPAREN argument_expression_list_opt RPAREN  */
#line 197 "safeguard.y"
    { (yyval.node_ptr) = create_function_call_node((yyvsp[-3].node_ptr), (yyvsp[-1].node_list_ptr), (yyvsp[-3].node_ptr)->line_number); }
#line 1657 "safeguard.tab.c"
    break;

  case 52: /* postfix_expression: PRINT_NUMBER_FN LPAREN expression RPAREN  */
#line 199 "safeguard.y"
    { ASTNode* func_name_node = create_identifier_node(strdup("print_number"), (yylsp[-3]).first_line);
      (yyval.node_ptr) = create_function_call_node(func_name_node, create_node_list((yyvsp[-1].node_ptr)), (yylsp[-3]).first_line); }
#line 1664 "safeguard.tab.c"
    break;

  case 53: /* postfix_expression: PRINT_TEXT_FN LPAREN expression RPAREN  */
#line 202 "safeguard.y"
    { ASTNode* func_name_node = create_identifier_node(strdup("print_text"), (yylsp[-3]).first_line);
      (yyval.node_ptr) = create_function_call_node(func_name_node, create_node_list((yyvsp[-1].node_ptr)), (yylsp[-3]).first_line); }
#line 1671 "safeguard.tab.c"
    break;

  case 54: /* argument_expression_list_opt: %empty  */
#line 207 "safeguard.y"
                                { (yyval.node_list_ptr) = NULL; }
#line 1677 "safeguard.tab.c"
    break;

  case 55: /* argument_expression_list_opt: argument_expression_list  */
#line 208 "safeguard.y"
                                { (yyval.node_list_ptr) = (yyvsp[0].node_list_ptr); }
#line 1683 "safeguard.tab.c"
    break;

  case 56: /* argument_expression_list: assignment_expression  */
#line 212 "safeguard.y"
                                { (yyval.node_list_ptr) = create_node_list((yyvsp[0].node_ptr)); }
#line 1689 "safeguard.tab.c"
    break;

  case 57: /* argument_expression_list: argument_expression_list ',' assignment_expression  */
#line 213 "safeguard.y"
                                                         { (yyval.node_list_ptr) = append_to_list((yyvsp[-2].node_list_ptr), (yyvsp[0].node_ptr)); }
#line 1695 "safeguard.tab.c"
    break;

  case 58: /* primary_expression: lvalue  */
#line 217 "safeguard.y"
           { (yyval.node_ptr) = (yyvsp[0].node_ptr); }
#line 1701 "safeguard.tab.c"
    break;

  case 59: /* primary_expression: NUMBER_LITERAL  */
#line 219 "safeguard.y"
    { (yyval.node_ptr) = create_number_literal_node((yyvsp[0].int_val), (yylsp[0]).first_line); }
#line 1707 "safeguard.tab.c"
    break;

  case 60: /* primary_expression: STRING_LITERAL  */
#line 221 "safeguard.y"
    { (yyval.node_ptr) = create_string_literal_node((yyvsp[0].str_val), (yylsp[0]).first_line); }
#line 1713 "safeguard.tab.c"
    break;

  case 61: /* primary_expression: LPAREN expression RPAREN  */
#line 223 "safeguard.y"
    { (yyval.node_ptr) = (yyvsp[-1].node_ptr); (yyvsp[-1].node_ptr)->line_number = (yylsp[-2]).first_line; /* Or $2 takes its own line, grouped expr can take LPAREN's line */ }
#line 1719 "safeguard.tab.c"
    break;

  case 62: /* primary_expression: READ_NUMBER_FN LPAREN RPAREN  */
#line 225 "safeguard.y"
    { (yyval.node_ptr) = create_read_number_call_node((yylsp[-2]).first_line); }
#line 1725 "safeguard.tab.c"
    break;

  case 63: /* primary_expression: SIZE_OF LPAREN IDENTIFIER RPAREN  */
#line 227 "safeguard.y"
    { (yyval.node_ptr) = create_size_of_call_node((yyvsp[-1].str_val), (yylsp[-3]).first_line); }
#line 1731 "safeguard.tab.c"
    break;

  case 64: /* lvalue: IDENTIFIER  */
#line 232 "safeguard.y"
    { (yyval.node_ptr) = create_identifier_node((yyvsp[0].str_val), (yylsp[0]).first_line); }
#line 1737 "safeguard.tab.c"
    break;

  case 65: /* lvalue: lvalue LBRACKET expression RBRACKET  */
#line 234 "safeguard.y"
    { (yyval.node_ptr) = create_array_access_node((yyvsp[-3].node_ptr), (yyvsp[-1].node_ptr), (yyvsp[-3].node_ptr)->line_number); }
#line 1743 "safeguard.tab.c"
    break;

  case 66: /* function_definition: DEFINE IDENTIFIER LPAREN RPAREN RETURNS type_specifier block_statement  */
#line 239 "safeguard.y"
    { (yyval.node_ptr) = create_func_def_node((yyvsp[-5].str_val), /* params */ (yyvsp[-1].node_ptr), (yyvsp[0].node_ptr), (yylsp[-6]).first_line); }
#line 1749 "safeguard.tab.c"
    break;


#line 1753 "safeguard.tab.c"

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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 242 "safeguard.y"


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
