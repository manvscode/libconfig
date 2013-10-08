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
#include "config-private.h"
#include "config.h"
}

%x COMMENT

whitespace     [ \t\r]

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
{whitespace}+                               { /* ignore whitespace */ }
\n                                          { config_t* p_config = config_get_extra( yyscanner ); config_increment_line(p_config); }
^\n                                         { /* ignore empty lines */ config_t* p_config = config_get_extra( yyscanner ); config_increment_line(p_config); }
{line_comments}                             { /* ignore line comments */ }
"/*"                                        { BEGIN COMMENT; /* enter the comment state */ }
<COMMENT>.*"*/"                             { BEGIN 0; /* leave the comment state */ }
<COMMENT>.                                  { /* ignore comment text */ } 
<COMMENT>\n                                 { /* ignore comment text */ config_t* p_config = config_get_extra( yyscanner ); config_increment_line(p_config); }
{name}/({whitespace}*"{"|{whitespace}*"=")  { 
												YYSTYPE *lval = config_get_lval( yyscanner );
												lval->string = strdup(yytext);
												return TOK_NAME;
                                            }
{integer}                                   { 
												YYSTYPE *lval = config_get_lval( yyscanner );
												lval->integer = atol(yytext);
												return TOK_INTEGER; 
                                            }
{decimal}                                   { 
												YYSTYPE *lval = config_get_lval( yyscanner );
												lval->decimal = atof(yytext);
												return TOK_DECIMAL;
                                            }
{boolean}                                   { 
												unsigned char b = strcasecmp(yytext, "true") == 0 || strcmp(yytext, "1") == 0; 
												YYSTYPE *lval = config_get_lval( yyscanner );
												lval->boolean = b;
												return TOK_BOOLEAN;
                                            }
{string}                                    { 
												YYSTYPE *lval = config_get_lval( yyscanner );
											
												size_t len = strlen( yytext ) - 2; /* minus the quotes */
												lval->string = (char*) malloc( len + 1 );
												strncpy( lval->string, yytext + 1, len );
												lval->string[ len ] = '\0';
												return TOK_STRING;
                                            }
"{"                                         { return '{'; }
"}"                                         { return '}'; }
"="                                         { return '='; }
.                                           { }

%%
int config_wrap (yyscan_t yyscanner )
{
	return 1;
}

void config_error( yyscan_t yyscanner, const char* error )
{
	config_t* p_config = config_get_extra( yyscanner );
	const char* text = config_get_text( yyscanner );

	fprintf( stderr, "Error: %s (line %d, problem around text \"%s\")\n", error, config_current_line(p_config), text );
	exit( -1 );
}

