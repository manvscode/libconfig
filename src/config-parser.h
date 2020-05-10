/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_CONFIG_SRC_CONFIG_PARSER_H_INCLUDED
# define YY_CONFIG_SRC_CONFIG_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int config_debug;
#endif
/* "%code requires" blocks.  */
#line 8 "src/config-parser.y" /* yacc.c:1909  */

#include <collections/variant.h>
#include <collections/tree-map.h>
typedef struct config_pair {
	const char* name;
	lc_variant_t*  value;
} config_pair_t;


#line 54 "src/config-parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_LCURLY = 258,
    TOK_RCURLY = 259,
    TOK_EQUAL = 260,
    TOK_NAME = 261,
    TOK_STRING = 262,
    TOK_INTEGER = 263,
    TOK_DECIMAL = 264,
    TOK_BOOLEAN = 265
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 34 "src/config-parser.y" /* yacc.c:1909  */

	char* string;
	long  integer;
	double decimal;
	#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
	bool boolean;
	#else
	unsigned char boolean;
	#endif
	config_pair_t pair;

#line 89 "src/config-parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int config_parse (yyscan_t scanner);

#endif /* !YY_CONFIG_SRC_CONFIG_PARSER_H_INCLUDED  */
