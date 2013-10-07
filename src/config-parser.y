/* bison -o config-parser.c -d -v --name-prefix=config_ config-parser.y */

%pure-parser
%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}


%code requires {
#include <libcollections/variant.h>
#include <libcollections/tree-map.h>
typedef struct config_pair {
	const char* name;
	lc_variant_t*  value;
} config_pair_t;

}

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#endif
#include <assert.h>
#include "config-private.h"
#include "config-lexer.h"
#include "config.h"

extern void config_error( yyscan_t yyscanner, const char* error );

%}

%union {
	char* string;
	long  integer;
	double decimal;
	#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
	bool boolean;
	#else
	unsigned char boolean;
	#endif
	config_pair_t pair;
}


%token TOK_LCURLY TOK_RCURLY TOK_EQUAL 
%token <string> TOK_NAME 
%token <string> TOK_STRING 
%token <integer> TOK_INTEGER 
%token <decimal> TOK_DECIMAL 
%token <boolean> TOK_BOOLEAN

%type <pair> groups
%type <pair> group
%type <pair> statements
%type <pair> statement

%%

/*
config : groups                            {
												config_t* p_config = config_get_extra( scanner );
												config_pair_t group = $1;
												lc_tree_map_t* main_group = variant_pointer( group.value );
												config_set_main_group( p_config, main_group );
                                           }
       ;
*/

groups : groups group                      {
												config_pair_t group1 = $1;
												config_pair_t group2 = $2;

												config_t* p_config = config_get_extra( scanner );
												lc_tree_map_t* main_group = config_main_group( p_config );

												if( !main_group )
												{
													main_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																							  (tree_map_compare_function) group_item_compare,
																							   malloc, free );
													config_set_main_group( p_config, main_group );
												}

												void* unused;
												if( !tree_map_find( main_group, group1.name, &unused ) )
												{
													tree_map_insert( main_group, group1.name, group1.value );
												}
												if( !tree_map_find( main_group, group2.name, &unused ) )
												{
													tree_map_insert( main_group, group2.name, group2.value );
												}

												/*
												config_pair_t group1 = $1;
												config_pair_t group2 = $2;

												lc_tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																						  (tree_map_compare_function) group_item_compare,
																						   malloc, free );
												tree_map_insert( p_new_group, group1.name, group1.value );
												tree_map_insert( p_new_group, group2.name, group2.value );

												config_pair_t enclosing_group;
												enclosing_group.name = strdup("todo"); 
												enclosing_group.value = variant_create( VARIANT_POINTER );
												variant_set_pointer( enclosing_group.value, p_new_group );
												$$ = enclosing_group;	
												*/

												/*
												config_pair_t group1 = $1;
												config_pair_t group2 = $2;
							
												lc_tree_map_t* p_group = variant_pointer(group1.value);
												tree_map_insert( p_group, group2.name, group2.value );
												$$ = group1;
												*/
                                           }
       | group                             {
												config_pair_t group = $1;
												$$ = group;

												config_t* p_config = config_get_extra( scanner );
												lc_tree_map_t* main_group = config_main_group( p_config );

												if( !main_group )
												{
													main_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																							  (tree_map_compare_function) group_item_compare,
																							   malloc, free );
													config_set_main_group( p_config, main_group );
												}

												void* unused;
												if( !tree_map_find( main_group, group.name, &unused ) )
												{
													tree_map_insert( main_group, group.name, group.value );
												}
                                           }
       ;

group : TOK_NAME '{' statements '}'        { 
												config_pair_t group = $3;
												group.name = $1;

												// group.group is already initialized from $2
												$$ = group;

												config_t* p_config = config_get_extra( scanner );
												if( config_is_verbose(p_config) )
												{
													printf( "Read Group: \"%s\" \n", $1 );
												}
										   }
      | TOK_NAME '{' '}'                   { 
												config_pair_t group;
												group.name = $1;
												group.value = NULL;
												$$ = group;

												config_t* p_config = config_get_extra( scanner );
												if( config_is_verbose(p_config) )
												{
													printf( "Read Group: \"%s\" \n", $1 );
												}
                                           }
      ;

statements : statements statement          { 
												config_pair_t group = $1;
												lc_tree_map_t* p_group = variant_pointer(group.value);
												assert( tree_map_size( p_group ) > 0 && tree_map_size(p_group) < 4096 );
												tree_map_insert( p_group, $2.name, $2.value );
												assert( tree_map_size( p_group ) > 0 && tree_map_size(p_group) < 4096 );
												$$ = group;
                                           }
           | statements group              {
												config_pair_t group1 = $1;
												config_pair_t group2 = $2;
												lc_tree_map_t* p_group = variant_pointer(group1.value);
												tree_map_insert( p_group, group2.name, group2.value );
												assert( tree_map_size( p_group ) > 0 && tree_map_size(p_group) < 4096 );
												$$ = group1;
                                           }
           | statement                     {
												lc_tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																						  (tree_map_compare_function) group_item_compare,
																						   malloc, free );
												tree_map_insert( p_new_group, $1.name, $1.value );

												assert( tree_map_size( p_new_group ) > 0 && tree_map_size(p_new_group) < 4096 );

												config_pair_t group;
												group.name = NULL; /* Will be set later */
												group.value = variant_create( VARIANT_POINTER );
												variant_set_pointer( group.value, p_new_group );
												$$ = group;	
                                           }
           | group                         {
												lc_tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
																						  (tree_map_compare_function) group_item_compare,
																						   malloc, free );
												tree_map_insert( p_new_group, $1.name, $1.value );
												assert( tree_map_size( p_new_group ) > 0 && tree_map_size(p_new_group) < 4096 );

												config_pair_t group;
												group.name = NULL; /* Will be set later */
												group.value = variant_create( VARIANT_POINTER );
												variant_set_pointer( group.value, p_new_group );
												$$ = group;	
                                           }
           ;


statement : TOK_NAME '=' TOK_STRING        { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_STRING );
												variant_set_string( pair.value, $3 );
												$$ = pair;
										
												config_t* p_config = config_get_extra( scanner );
												if( config_is_verbose(p_config) )
												{
													printf( "Read %s: \"%s\" \n", $1, $3 );
												}
                                           }
          | TOK_NAME '=' TOK_INTEGER       { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_INTEGER );
												variant_set_integer( pair.value, $3 );
												$$ = pair;

												config_t* p_config = config_get_extra( scanner );
												if( config_is_verbose(p_config) )
												{
													printf( "Read %s: %ld \n", $1, $3 );
												}
                                           }
          | TOK_NAME '=' TOK_DECIMAL       { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_DECIMAL );
												variant_set_decimal( pair.value, $3 );
												$$ = pair;

												config_t* p_config = config_get_extra( scanner );
												if( config_is_verbose(p_config) )
												{
													printf( "Read %s: %lf \n", $1, $3 );
												}
                                           }
          | TOK_NAME '=' TOK_BOOLEAN       { 
												config_pair_t pair;
												pair.name  = $1;
												pair.value = variant_create( VARIANT_BOOLEAN );
												variant_set_boolean( pair.value, $3 );
												$$ = pair;

												config_t* p_config = config_get_extra( scanner );
												if( config_is_verbose(p_config) )
												{
													printf( "Read %s: %s \n", $1, $3 ? "true" : "false" );
												}
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

