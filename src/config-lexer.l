/* flex -o config-lexer.c --prefix=config_  --header-file=config-lexer.h config-lexer.l */
%{
#include <stdlib.h>
#include <stdio.h>
#include "config-parser.h"
%}

%x COMMENT

whitespace     [ \t\r]+

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

{whitespace}              { /* ignore whitespace */ }
^\n                       { /* ignore empty lines */ }
{line_comments}           { /* ignore line comments */ }
"/*"                      { BEGIN COMMENT; /* enter the comment state */ }
<COMMENT>.*"*/"           { BEGIN 0; /* leave the comment state */ }
<COMMENT>(.|\n)           { /* ignore comment text */ } 

{name}                  { if( 0 ) printf( "   name = '%s'\n", yytext); config_lval.string = strdup(yytext); return TOK_NAME; }
{integer}$               { if( 0 ) printf( "    int = '%s'\n", yytext); config_lval.integer = atol(yytext); return TOK_INTEGER; }
{decimal}$               { if( 0 ) printf( "decimal = '%s'\n", yytext); config_lval.decimal = atof(yytext); return TOK_DECIMAL; }
{boolean}$              { if( 0 ) printf( "   bool = '%s'\n", yytext); config_lval.boolean = strcasecmp(yytext, "true") == 0 || strcmp(yytext, "1") == 0; return TOK_BOOLEAN; }
{string}                { if( 0 ) printf( " string = '%s'\n", yytext); config_lval.string = strdup(yytext); return TOK_STRING; }
"{"                     { if( 0 ) printf( "{\n", yytext); return '{'; }
"}"                     { if( 0 ) printf( "}\n", yytext); return '}'; }
"="                     { if( 0 ) printf( "=\n", yytext); return '='; }
.                       { if( 1 ) printf( "ignoring = '%s'\n", yytext); }

%%

/*
int main( int argc, char* argv[] )
{
	config_lex();
	return 0;
}
*/

int config_wrap()
{
	return 1;
}

void config_error( const char* error )
{
	fflush(stdout);
	fprintf( stderr, "%s\n", error );
}

