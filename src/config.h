#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <libcollections/types.h>
#include <libcollections/variant.h>
#include <libcollections/tree-map.h>


struct config;
typedef struct config config_t;


config_t*     config_create      ( const char* filename, boolean verbose );
void          config_destroy     ( config_t** p_config );
lc_variant_t* config_find        ( config_t* p_config, const char* key );
void          config_dump        ( FILE* file, const config_t* p_config );
boolean       config_is_verbose  ( config_t* p_config );
void          config_set_verbose ( config_t* p_config, boolean v );

#if 0
#define config_get( p_config, key )  _Generic( variant_type(config_find(p_config, key)), \
	VARIANT_STRING: variant_value(config_find(p_config, key)).string,  \
	VARIANT_DECIMAL: variant_value(config_find(p_config, key)).decimal,  \
	VARIANT_INTEGER: variant_value(config_find(p_config, key)).integer,  \
	VARIANT_UNSIGNED_INTEGER: variant_value(config_find(p_config, key)).unsigned_integer,  \
	VARIANT_POINTER: variant_value(config_find(p_config, key)).pointer,  \
	VARIANT_NOT_INITIALIZED: NULL,\
	default: 0 \
	) (p_config, key)
#endif



#endif
