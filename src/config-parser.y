/* bison -o config-parser.c -d -v --name-prefix=config_ config-parser.y */

%pure-parser
%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}


%code requires {
#include <libcollections/variant.h>
#include <libcollections/tree-map.h>
typedef struct config_pair {
	const char* name;
	variant_t*  value;
} config_pair_t;

typedef struct config_group {
	const char* name;
	tree_map_t* group;
} config_group_t;
}

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config-private.h"
#include "config-lexer.h"
#include "config.h"

extern void config_error( yyscan_t yyscanner, const char* error );

%}

%union {
	char* string;
	long  integer;
	double decimal;
	unsigned char boolean;
	config_group_t group;
	config_pair_t pair;
}


%token TOK_LCURLY TOK_RCURLY TOK_EQUAL 
%token <string> TOK_NAME 
%token <string> TOK_STRING 
%token <integer> TOK_INTEGER 
%token <decimal> TOK_DECIMAL 
%token <boolean> TOK_BOOLEAN

%type <group> groups
%type <group> group
%type <group> statements
%type <pair> statement

%%


config : groups                            {
												config_t* p_config = config_get_extra( scanner );
												tree_map_t* main_group = $1.group;
												config_set_main_group( p_config, main_group );
                                           }
       ;

groups : groups group                      {
												config_group_t group = $1;
												tree_map_t* p_group = group.group;
												tree_map_insert( p_group, $2.name, $2.group );
												$$ = $1;
                                           }
       | group                             {
												$$ = $1;
                                           }
       ;

group : TOK_NAME '{' statements '}'        { 
												config_group_t group = $<group>2;
												group.name = $1;
												// group.group is already initialized from $2
												$$ = group;
												printf( " Read Group: %s \n", $1 );
										   }
      | TOK_NAME '{' '}'                   { 
												config_group_t group;
												group.name = $1;
												group.group = NULL;
												$$ = group;
												printf( " Read Group: %s \n", $1 );
                                           }
      ;

statements : statements statement          { 
												config_group_t group = $1;
												tree_map_t* p_group = group.group;
												tree_map_insert( p_group, $2.name, $2.value );
												$$ = $1;
                                           }
           | statements group              {
												config_group_t group = $1;
												tree_map_t* p_group = group.group;
												tree_map_insert( p_group, $2.name, $2.group );
												$$ = $1;
                                           }
           | statement                     {
												tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																						  (tree_map_compare_function) group_item_compare,
																						   malloc, free );
												tree_map_insert( p_new_group, $1.name, $1.value );

												config_group_t group;
												group.name = NULL; /* Will be set later */
												group.group = p_new_group;
												$$ = group;	
                                           }
           | group                         {
												tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																						  (tree_map_compare_function) group_item_compare,
																						   malloc, free );
												tree_map_insert( p_new_group, $1.name, $1.group );

												config_group_t group;
												group.name = NULL; /* Will be set later */
												group.group = p_new_group;
												$$ = group;	
                                           }
           ;


statement : TOK_NAME '=' TOK_STRING        { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_STRING );
												variant_set_string( pair.value, $3 );
												$$ = pair;
												printf( "Read %s: %s \n", $1, $3 ); 
                                           }
          | TOK_NAME '=' TOK_INTEGER       { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_INTEGER );
												variant_set_integer( pair.value, $3 );
												$$ = pair;
												printf( "Read %s: %ld \n", $1, $3 ); 
                                           }
          | TOK_NAME '=' TOK_DECIMAL       { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_DECIMAL );
												variant_set_decimal( pair.value, $3 );
												$$ = pair;
												printf( "Read %s: %lf \n", $1, $3 );
                                           }
          | TOK_NAME '=' TOK_BOOLEAN       { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_UNSIGNED_INTEGER );
												variant_set_unsigned_integer( pair.value, $3 );
												$$ = pair;
												printf( "Read %s: %d \n", $1, $3 ); 
                                           }
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

