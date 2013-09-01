#ifndef _CONFIG_H_
#define _CONFIG_H_




struct config;
typedef struct config config_t;


config_t*  config_create  ( const char* filename );
void       config_destroy ( config_t** p_config );
variant_t* config_find    ( config_t* p_config, const char* key );


#define config_get( p_config, key )  _Generic( variant_type(config_find(p_config, key)), \
	VARIANT_STRING: variant_value(config_find(p_config, key)).string,  \
	VARIANT_DECIMAL: variant_value(config_find(p_config, key)).decimal,  \
	VARIANT_INTEGER: variant_value(config_find(p_config, key)).integer,  \
	VARIANT_UNSIGNED_INTEGER: variant_value(config_find(p_config, key)).unsigned_integer,  \
	VARIANT_POINTER: variant_value(config_find(p_config, key)).pointer,  \
	VARIANT_NOT_INITIALIZED: NULL,\
	default: 0 \
	) (p_config, key)

bool config_add_group   ( config_t* p_config, const char* name );
bool config_add_setting ( tree_map_t* p_group, const char* name, const variant_t* p_variant );
bool config_add_string  ( tree_map_t* p_group, const char* name, const char* value );
bool config_add_boolean ( tree_map_t* p_group, const char* name, bool value );
bool config_add_integer ( tree_map_t* p_group, const char* name, long value );
bool config_add_decimal ( tree_map_t* p_group, const char* name, double value );


#endif
