/* bison -o config-parser.c -d -v --name-prefix=config_ config-parser.y */

%pure-parser
%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config-lexer.h"
extern void config_error( yyscan_t yyscanner, const char* error );
%}

%union {
	char* string;
	long  integer;
	double decimal;
	unsigned char boolean;
}
%token TOK_LCURLY TOK_RCURLY TOK_EQUAL 
%token <string> TOK_NAME 
%token <string> TOK_STRING 
%token <integer> TOK_INTEGER 
%token <decimal> TOK_DECIMAL 
%token <boolean> TOK_BOOLEAN

%%

groups : groups group
       | group            
       ;

group : TOK_NAME '{' statements '}'        { 
												printf( "group := %s \n", $1 );
										   }
      | TOK_NAME '{' '}'                   { printf( "group := %s \n", $1 ); }
      ;

statements : statements statement
           | statements group
           | statement                  
           | group                  
           ;


statement : TOK_NAME '=' TOK_STRING        { 
												printf( "%30s := %s \n", $1, $3 ); 
											}
          | TOK_NAME '=' TOK_INTEGER       { printf( "%30s := %ld \n", $1, $3 ); }
          | TOK_NAME '=' TOK_DECIMAL       { printf( "%30s := %lf \n", $1, $3 ); }
          | TOK_NAME '=' TOK_BOOLEAN       { printf( "%30s := %d \n", $1, $3 ); }
          ;
%%

/*
int main( int argc, char* argv[] )
{
	config_in = fopen( argv[1], "r" );

	do {
		config_parse();
	} while( !feof(config_in) );

	return 0;
}
*/

