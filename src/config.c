#include <libcollections/tree-map.h>
#include <libcollections/variant.h>


struct config {
	tree_map_t groups;

};

static boolean group_item_destroy ( const char* *p_key, variant_t *p_value );
static int     group_item_compare ( const char* p_key_left, const char* p_key_right );


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
}

int group_item_compare( const char* p_key_left, const char* p_key_right )
{
	return strcmp( p_key_left, p_key_right );
}



config_t* config_create( const char* filename )
{
	config_t* p_config = (config_t*) malloc( sizeof(config_t) );

	if( p_config )
	{
		p_config->groups;
		tree_map_create( &p_config->groups,
						(tree_map_element_function) group_item_destroy,
						(tree_map_compare_function) group_item_compare,
						malloc, free );
	}

	return p_config;
}


// settings.network.host
variant_t* config_find( config_t* p_config, const char* key )
{
	tree_map_t* p_group = &p_config->groups;
	const char current_key[ 256 ] = { '\0' };
	variant_t* p_result = NULL;

	strcpy( key_copy, key );

	const char* current_key = strchr( key_copy, '.' );

	while( current_key )
	{
		*current_key = '\0';

		if( tree_map_find( p_group, current_key, &p_result ) )
		{
			if( variant_is_pointer( p_result ) )
			{
				p_group = (tree_map_t*) variant_pointer( p_result );
				current_key++; // skip over null terminator
				current_key = strchr( current_key, '.' );
			}
		}
		else
		{
			current_key = NULL;
		}
	}

	return p_result;
}



bool config_add_group( config_t* p_config, const char* name )
{

	tree_map_t* p_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
	                                          (tree_map_compare_function) group_item_compare,
	                                           malloc, free );

	variant_t* p_variant = variant_create_pointer( );
	variant_set_pointer( p_variant, p_group );
	variant_set_value( p_variant, value );

	return config_add_setting( &p_config->groups, name, p_variant );
}

bool config_add_group_to_group( config_t* p_config, tree_map_t* p_group, const char* name )
{

	tree_map_t* p_new_group = tree_map_create_ex( (tree_map_element_function) group_item_destroy,
	                                          (tree_map_compare_function) group_item_compare,
	                                           malloc, free );

	variant_t* p_variant = variant_create_pointer( );
	variant_set_pointer( p_variant, p_new_group );
	variant_set_value( p_variant, value );

	return config_add_setting( p_group, name, p_variant );
}

bool config_add_setting( tree_map_t* p_group, const char* name, const variant_t* p_variant )
{
	return tree_map_insert( p_group, strdup(name), p_variant );
}

bool config_add_string( tree_map_t* p_group, const char* name, const char* value )
{
	variant_t* p_variant = variant_create_unsigned_integer( );
	variant_set_string( p_variant, strdup(value) );
	return config_add_setting( &group, name, p_variant );
}

bool config_add_boolean( tree_map_t* p_group, const char* name, bool value )
{
	variant_t* p_variant = variant_create_unsigned_integer( );
	variant_set_unsigned_integer( p_variant, value );
	return config_add_setting( &group, name, p_variant );
}

bool config_add_integer( tree_map_t* p_group, const char* name, long value )
{
	variant_t* p_variant = variant_create_integer( );
	variant_set_integer( p_variant, value );
	return config_add_setting( &group, name, p_variant );
}

bool config_add_decimal( tree_map_t* p_group, const char* name, double value )
{
	variant_t* p_variant = variant_create_integer( );
	variant_set_decimal( p_variant, value );
	return config_add_setting( &group, name, p_variant );
}
