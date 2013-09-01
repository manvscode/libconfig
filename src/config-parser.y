/* bison -o config-parser.c -d --name-prefix=config_ config-parser.y */

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

%{
#include <stdio.h>

/*
extern bool config_add_group   ( config_t* p_config, const char* name );
extern bool config_add_setting ( tree_map_t* p_group, const char* name, const variant_t* p_variant );
extern bool config_add_string  ( tree_map_t* p_group, const char* name, const char* value );
extern bool config_add_boolean ( tree_map_t* p_group, const char* name, bool value );
extern bool config_add_integer ( tree_map_t* p_group, const char* name, long value );
extern bool config_add_decimal ( tree_map_t* p_group, const char* name, double value );

config_t config;
*/
%}


%%

groups : groups group
       | group            
       ;

group : TOK_NAME '{' statements '}'        { printf( "group := %s \n", $1 ); }
      | TOK_NAME '{' '}'                   { printf( "group := %s \n", $1 ); }
      ;

statements : statements statement
           | statements group
           | statement                  
           | group                  
           ;


statement : TOK_NAME '=' TOK_STRING        { printf( "%30s := %s \n", $1, $3 ); }
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

