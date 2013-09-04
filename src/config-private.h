#ifndef _CONFIG_PRIVATE_H_
#define _CONFIG_PRIVATE_H_

#include <libcollections/types.h>
#include <libcollections/variant.h>
#include "config.h"



boolean     group_item_destroy     ( const char* *p_key, variant_t *p_value );
int         group_item_compare     ( const char* p_key_left, const char* p_key_right );
tree_map_t* config_main_group      ( const config_t* p_config );
void        config_set_main_group  ( config_t* p_config, const tree_map_t* group );
void        config_increment_line  ( config_t* p_config );
unsigned int config_current_line   ( config_t* p_config );

/* TODO: Expose a way to manipulate the settings data structure 
 * so that it may be saved to a file stream. The pieces are there
 * but it isn't tested.
 */
boolean config_add_group   ( config_t* p_config, const char* name );
boolean config_add_setting ( tree_map_t* p_group, const char* name, const variant_t* p_variant );
boolean config_add_string  ( tree_map_t* p_group, const char* name, const char* value );
boolean config_add_boolean ( tree_map_t* p_group, const char* name, boolean value );
boolean config_add_integer ( tree_map_t* p_group, const char* name, long value );
boolean config_add_decimal ( tree_map_t* p_group, const char* name, double value );


#endif /* _CONFIG_PRIVATE_H_ */
