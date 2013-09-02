#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libcollections/tree-map.h>
#include <libcollections/variant.h>
#include <libcollections/types.h>
#include "config-lexer.h"
#include "config-parser.h"
#include "config.h"




struct config {
	boolean verbose;
	unsigned int line;
	tree_map_t* groups;
};



boolean group_item_destroy( const char* *p_key, variant_t *p_value )
{
	free( p_key );

	if( variant_is_string( p_value ) )
	{
		char* p_string = (char*) variant_string( p_value );
		free( p_string );
	}
	else if( variant_is_pointer( p_value ) )
	{
		tree_map_t* p_group = (tree_map_t*) variant_pointer( p_value );
		tree_map_destroy( p_group );
	}

	variant_destroy( p_value );

	return TRUE;
}

int group_item_compare( const char* p_key_left, const char* p_key_right )
{
	return strcmp( p_key_left, p_key_right );
}



config_t* config_create( const char* filename, boolean verbose )
{
	config_t* p_config = (config_t*) malloc( sizeof(config_t) );

	if( p_config )
	{
		#if 0
		p_config->groups = tree_map_create_ex(
						(tree_map_element_function) group_item_destroy,
						(tree_map_compare_function) group_item_compare,
						malloc, free );
		#endif
		p_config->line    = 1;
		p_config->verbose = verbose;

		FILE* file = fopen( filename, "r" );
		yyscan_t scanner;
		config_lex_init( &scanner );
		config_set_in( file, scanner );
		config_set_extra( p_config, scanner );
		#if 0
		config_set_debug( 1, scanner );
		config_debug = 1;
		#endif

		do {
			config_parse( scanner );
		} while( !feof(file) );

		config_lex_destroy( scanner );
	}

	return p_config;
}

void config_destroy( config_t** p_config )
{
	if( *p_config )
	{
		tree_map_destroy( (*p_config)->groups );
		free( (*p_config)->groups );

		free( *p_config );
		*p_config = NULL;
	}
}

tree_map_t* config_main_group( const config_t* p_config )
{
	assert( p_config );
	return p_config->groups;
}

void config_set_main_group( config_t* p_config, const tree_map_t* group )
{
	assert( p_config );
	p_config->groups = (tree_map_t*) group;
}

unsigned int config_current_line( config_t* p_config )
{
	assert( p_config );
	return p_config->line;
}

void config_increment_line( config_t* p_config )
{
	assert( p_config );
	p_config->line++;
}

// settings.network.host
variant_t* config_find( config_t* p_config, const char* key )
{
	tree_map_t* p_group = p_config->groups;
	char key_copy[ 256 ] = { '\0' };
	variant_t* p_result = NULL;

	strcpy( key_copy, key );

	char* dot = strchr( key_copy, '.' );
	char* current_key = key_copy;

	while( current_key )
	{
		if( dot )
		{
			*dot = '\0';
		}

		if( tree_map_find( p_group, current_key, (void**) &p_result ) )
		{
			if( variant_is_pointer( p_result ) )
			{
				p_group = (tree_map_t*) variant_pointer( p_result );
				if( dot )
				{
					current_key = dot + 1; // skip over null terminator
					dot = strchr( current_key, '.' );
				}
			}
			else
			{
				/* found something so we're done searching. */
				current_key = NULL;
			}
		}
		else
		{
			p_result    = NULL;
			current_key = NULL;
		}
	}

	return p_result;
}

void config_dump_tab( FILE* file, short indent )
{
	while( indent > 0 )
	{
		fprintf( file, "\t" );
		indent--;
	}
}

void config_dump_group( FILE* file, short indent, const char* name, const tree_map_t* group )
{
	if( name )
	{
		config_dump_tab( file, indent );
		fprintf( file, "%s {\n", name );
	}

	for( tree_map_iterator_t itr = tree_map_begin( group );
	     itr != tree_map_end( );
		 itr = tree_map_next( itr ) )
	{
		const char* name       = itr->key;
		const variant_t* value = itr->value;

		switch( variant_type(value) )
		{
			case VARIANT_POINTER: /* tree_map_t* */
				config_dump_group( file, indent + 1, name, variant_pointer(value) );
				break;
			case VARIANT_STRING:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = \"%s\"\n", name, variant_string(value) );
				break;
			case VARIANT_DECIMAL:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = %lf\n", name, variant_decimal(value) );
				break;
			case VARIANT_INTEGER:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = %ld\n", name, variant_integer(value) );
				break;
			case VARIANT_UNSIGNED_INTEGER:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = %ld\n", name, variant_unsigned_integer(value) );
				break;
			default:
				/* Ignore unknown values */
				fprintf( stderr, "Error: Ignoring unknown config setting \"%s\" of type %d.\n", name, variant_type(value) );
				break;
		}
	}

	if( name )
	{
		config_dump_tab( file, indent );
		fprintf( file, "}\n" );
	}
}


void config_dump( FILE* file, const config_t* p_config )
{
	short indent = -1;
	config_dump_group( file, indent, NULL, p_config->groups );
}

boolean config_is_verbose( config_t* p_config )
{
	assert( p_config );
	return p_config->verbose;
}

void config_set_verbose( config_t* p_config, boolean v )
{
	assert( p_config );
	p_config->verbose = v;
}


boolean config_add_group( config_t* p_config, const char* name )
{

	tree_map_t* p_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
	                                          (tree_map_compare_function) group_item_compare,
	                                           malloc, free );

	variant_t* p_variant = variant_create_pointer( );
	variant_set_pointer( p_variant, p_group );

	return config_add_setting( p_config->groups, name, p_variant );
}

boolean config_add_group_to_group( config_t* p_config, tree_map_t* p_group, const char* name )
{

	tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
	                                          (tree_map_compare_function) group_item_compare,
	                                           malloc, free );

	variant_t* p_variant = variant_create_pointer( );
	variant_set_pointer( p_variant, p_new_group );

	return config_add_setting( p_group, name, p_variant );
}

boolean config_add_setting( tree_map_t* p_group, const char* name, const variant_t* p_variant )
{
	const char* name_copy = strdup(name);
	return tree_map_insert( p_group, name_copy, p_variant );
}

boolean config_add_string( tree_map_t* p_group, const char* name, const char* value )
{
	variant_t* p_variant = variant_create_unsigned_integer( );
	const char* value_copy = strdup(value);
	variant_set_string( p_variant, value_copy );
	return config_add_setting( p_group, name, p_variant );
}

boolean config_add_boolean( tree_map_t* p_group, const char* name, boolean value )
{
	variant_t* p_variant = variant_create_unsigned_integer( );
	variant_set_unsigned_integer( p_variant, value );
	return config_add_setting( p_group, name, p_variant );
}

boolean config_add_integer( tree_map_t* p_group, const char* name, long value )
{
	variant_t* p_variant = variant_create_integer( );
	variant_set_integer( p_variant, value );
	return config_add_setting( p_group, name, p_variant );
}

boolean config_add_decimal( tree_map_t* p_group, const char* name, double value )
{
	variant_t* p_variant = variant_create_integer( );
	variant_set_decimal( p_variant, value );
	return config_add_setting( p_group, name, p_variant );
}
