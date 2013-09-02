/* flex -F -nounistd -R -o config-lexer.c --prefix=config_  --header-file=config-lexer.h config-lexer.l */

%option reentrant never-interactive bison-bridge nounistd 

%top{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libcollections/variant.h>
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
#include "config-parser.h"
}

%x COMMENT

whitespace     [ \t\r\n]

integer        [0-9]+
decimal        [0-9]*\.[0-9]+
boolean        (true|false|True|False|TRUE|FALSE)
name           [_a-zA-Z0-9]+
string         \"(\\.|[^\\"])*\"

line_comments  ("//"+.*)
lcurly         "{"
rcurly         "}"
equal          "="

%%
{whitespace}+             { /* ignore whitespace */ }
^\n                       { /* ignore empty lines */ }
{line_comments}           { /* ignore line comments */ }
"/*"                      { BEGIN COMMENT; /* enter the comment state */ }
<COMMENT>.*"*/"           { BEGIN 0; /* leave the comment state */ }
<COMMENT>(.|\n)           { /* ignore comment text */ } 

{name}/({whitespace}*"{"|{whitespace}*"=")                { 
								YYSTYPE *lval = config_get_lval( yyscanner );
								lval->string = strdup(yytext);
								return TOK_NAME;
                          }
{integer}                 { 
								YYSTYPE *lval = config_get_lval( yyscanner );
								lval->integer = atol(yytext);
								//variant_t* p_variant = variant_create( VARIANT_INTEGER );
								//variant_set_integer( p_variant, atol(yytext) );
								//config_set_lval( p_variant, yyscanner ); 
								return TOK_INTEGER; 
                          }
{decimal}                 { 
								YYSTYPE *lval = config_get_lval( yyscanner );
								lval->decimal = atof(yytext);
								//variant_t* p_variant = variant_create( VARIANT_DECIMAL );
								//variant_set_decimal( p_variant, atof(yytext) );
								//config_set_lval( p_variant, yyscanner ); 
								return TOK_DECIMAL;
                          }
{boolean}                 { 
								unsigned char b = strcasecmp(yytext, "true") == 0 || strcmp(yytext, "1") == 0; 
								YYSTYPE *lval = config_get_lval( yyscanner );
								lval->boolean = b;
								//variant_t* p_variant = variant_create( VARIANT_UNSIGNED_INTEGER );
								//variant_set_unsigned_integer( p_variant, b );
								//config_set_lval( p_variant, yyscanner ); 
								return TOK_BOOLEAN;
                          }
{string}                  { 
								YYSTYPE *lval = config_get_lval( yyscanner );
								lval->string = strdup(yytext);
								//variant_t* p_variant = variant_create( VARIANT_STRING );
								//variant_set_string( p_variant, strdup(yytext) );
								//config_set_lval( p_variant, yyscanner ); 
								return TOK_STRING;
                          }
"{"                       { return '{'; }
"}"                       { return '}'; }
"="                       { return '='; }
.                         { }

%%
int config_wrap (yyscan_t yyscanner )
{
	return 1;
}

void config_error( yyscan_t yyscanner, const char* error )
{
 	//int line = config_get_lineno( yyscanner );
	const char* text = config_get_text( yyscanner );

	fprintf( stderr, "Error: %s (text = \"%s\")\n", error, text );
	exit( -1 );
}

