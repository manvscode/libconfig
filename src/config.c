/*
 * Copyright (C) 2013 Joseph A. Marrero.   http://www.manvscode.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <collections/tree-map.h>
#include <collections/variant.h>
//#include <collections/types.h>
#include "config-lexer.h"
#include "config-parser.h"
#include "config.h"

struct config {
	bool verbose;
	unsigned int line;
	lc_tree_map_t* groups;
};



config_t* config_create( const char* filename, bool verbose )
{
	FILE* file = fopen( filename, "r" );

	if( !file )
	{
		goto failure;
	}

	config_t* p_config = malloc( sizeof(config_t) );

	if( p_config )
	{
		#if 1
		p_config->groups = lc_tree_map_create_ex(
				(lc_tree_map_element_fxn_t) group_item_destroy,
				(lc_tree_map_compare_fxn_t) group_item_compare,
				malloc, free );
		#else
		p_config->groups  = NULL;
		#endif
		p_config->line    = 1;
		p_config->verbose = verbose;


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

	fclose( file );

	return p_config;

failure:
	return NULL;
}

void config_destroy( config_t** p_config )
{
	if( *p_config )
	{
		lc_tree_map_destroy( (*p_config)->groups );
		free( (*p_config)->groups );

		free( *p_config );
		*p_config = NULL;
	}
}

lc_tree_map_t* config_main_group( const config_t* p_config )
{
	assert( p_config );
	return p_config->groups;
}

void config_set_main_group( config_t* p_config, const lc_tree_map_t* group )
{
	assert( p_config );
	p_config->groups = (lc_tree_map_t*) group;
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
lc_variant_t* config_find( const config_t* p_config, const char* key )
{
	lc_tree_map_t* p_group = p_config->groups;
	char key_copy[ 256 ] = { '\0' };
	lc_variant_t* p_result = NULL;

	strcpy( key_copy, key );

	char* dot = strchr( key_copy, '.' );
	char* current_key = key_copy;

	while( current_key )
	{
		if( dot )
		{
			*dot = '\0';
		}

		//lc_tree_map_iterator_t itr = lc_tree_map_find( p_group, current_key);
		//if( itr != lc_tree_map_end() )
		if( lc_tree_map_search( p_group, current_key, (void**) &p_result ) )
		{
			if( lc_variant_is_pointer( p_result ) )
			{
				p_group = (lc_tree_map_t*) lc_variant_pointer( p_result );
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

const char* config_find_string( const config_t* p_config, const char* key, const char* default_value )
{
	lc_variant_t* p_result = config_find( p_config, key );

	if( p_result )
	{
		return lc_variant_string( p_result );
	}

	return default_value;
}

long config_find_integer( const config_t* p_config, const char* key, long default_value )
{
	lc_variant_t* p_result = config_find( p_config, key );

	if( p_result )
	{
		return lc_variant_integer( p_result );
	}

	return default_value;
}

unsigned long config_find_unsigned_integer( const config_t* p_config, const char* key, unsigned long default_value )
{
	lc_variant_t* p_result = config_find( p_config, key );

	if( p_result )
	{
		return lc_variant_unsigned_integer( p_result );
	}

	return default_value;
}

double config_find_decimal( const config_t* p_config, const char* key, double default_value )
{
	lc_variant_t* p_result = config_find( p_config, key );

	if( p_result )
	{
		return lc_variant_decimal( p_result );
	}

	return default_value;
}

bool config_find_boolean( const config_t* p_config, const char* key, bool default_value )
{
	lc_variant_t* p_result = config_find( p_config, key );

	if( p_result )
	{
		return lc_variant_boolean( p_result );
	}

	return default_value;
}

void config_dump_tab( FILE* file, short indent )
{
	while( indent > 0 )
	{
		fprintf( file, "\t" );
		indent--;
	}
}

void config_dump_group( FILE* file, short indent, const char* name, const lc_tree_map_t* group )
{
	if( name )
	{
		config_dump_tab( file, indent );
		fprintf( file, "%s {\n", name );
	}

	for( lc_tree_map_iterator_t itr = lc_tree_map_begin( group );
	     itr != lc_tree_map_end( );
		 itr = lc_tree_map_next( itr ) )
	{
		const char* name       = itr->key;
		const lc_variant_t* value = itr->value;

		switch( lc_variant_type(value) )
		{
			case LC_VARIANT_POINTER: /* lc_tree_map_t* */
				config_dump_group( file, indent + 1, name, lc_variant_pointer(value) );
				break;
			case LC_VARIANT_STRING:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = \"%s\"\n", name, lc_variant_string(value) );
				break;
			case LC_VARIANT_DECIMAL:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = %lf\n", name, lc_variant_decimal(value) );
				break;
			case LC_VARIANT_INTEGER:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = %ld\n", name, lc_variant_integer(value) );
				break;
			case LC_VARIANT_UNSIGNED_INTEGER:
				config_dump_tab( file, indent + 1 );
				fprintf( file, "%s = %ld\n", name, lc_variant_unsigned_integer(value) );
				break;
			default:
				/* Ignore unknown values */
				fprintf( stderr, "Error: Ignoring unknown config setting \"%s\" of type %d.\n", name, lc_variant_type(value) );
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

bool config_is_verbose( const config_t* p_config )
{
	assert( p_config );
	return p_config->verbose;
}

void config_set_verbose( config_t* p_config, bool v )
{
	assert( p_config );
	p_config->verbose = v;
}


bool config_add_group( config_t* p_config, const char* name )
{

	lc_tree_map_t* p_group = lc_tree_map_create_ex( (lc_tree_map_element_fxn_t) group_item_destroy,
	                                          (lc_tree_map_compare_fxn_t) group_item_compare,
	                                           malloc, free );

	lc_variant_t* p_variant = lc_variant_create_pointer( );
	lc_variant_set_pointer( p_variant, p_group );

	return config_add_setting( p_config->groups, name, p_variant );
}

bool config_add_group_to_group( config_t* p_config, lc_tree_map_t* p_group, const char* name )
{

	lc_tree_map_t* p_new_group = lc_tree_map_create_ex( (lc_tree_map_element_fxn_t) group_item_destroy,
	                                          (lc_tree_map_compare_fxn_t) group_item_compare,
	                                           malloc, free );

	lc_variant_t* p_variant = lc_variant_create_pointer( );
	lc_variant_set_pointer( p_variant, p_new_group );

	return config_add_setting( p_group, name, p_variant );
}

bool config_add_setting( lc_tree_map_t* p_group, const char* name, const lc_variant_t* p_variant )
{
	const char* name_copy = strdup(name);
	return lc_tree_map_insert( p_group, name_copy, p_variant );
}

bool config_add_string( lc_tree_map_t* p_group, const char* name, const char* value )
{
	lc_variant_t* p_variant = lc_variant_create_unsigned_integer( );
	const char* value_copy = strdup(value);
	lc_variant_set_string( p_variant, value_copy );
	return config_add_setting( p_group, name, p_variant );
}

bool config_add_boolean( lc_tree_map_t* p_group, const char* name, bool value )
{
	lc_variant_t* p_variant = lc_variant_create_unsigned_integer( );
	lc_variant_set_unsigned_integer( p_variant, value );
	return config_add_setting( p_group, name, p_variant );
}

bool config_add_integer( lc_tree_map_t* p_group, const char* name, long value )
{
	lc_variant_t* p_variant = lc_variant_create_integer( );
	lc_variant_set_integer( p_variant, value );
	return config_add_setting( p_group, name, p_variant );
}

bool config_add_decimal( lc_tree_map_t* p_group, const char* name, double value )
{
	lc_variant_t* p_variant = lc_variant_create_integer( );
	lc_variant_set_decimal( p_variant, value );
	return config_add_setting( p_group, name, p_variant );
}

bool group_item_destroy( char* p_key, lc_variant_t* p_value )
{
	free( p_key );

	if( p_value )
	{
		if( lc_variant_is_string( p_value ) )
		{
			char* p_string = (char*) lc_variant_string( p_value );
			free( p_string );
		}
		else if( lc_variant_is_pointer( p_value ) )
		{
			lc_tree_map_t* p_group = (lc_tree_map_t*) lc_variant_pointer( p_value );
			lc_tree_map_destroy( p_group );
		}

		lc_variant_destroy( p_value );
	}

	return true;
}

int group_item_compare( const char* p_key_left, const char* p_key_right )
{
	return strcmp( p_key_left, p_key_right );
}
