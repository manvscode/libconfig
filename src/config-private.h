#ifndef _CONFIG_PRIVATE_H_
#define _CONFIG_PRIVATE_H_

//#include <libcollections/types.h>
#include <libcollections/variant.h>
#include "config.h"



bool           group_item_destroy     ( char* p_key, lc_variant_t *p_value );
int            group_item_compare     ( const char* p_key_left, const char* p_key_right );
lc_tree_map_t* config_main_group      ( const config_t* p_config );
void           config_set_main_group  ( config_t* p_config, const lc_tree_map_t* group );
void           config_increment_line  ( config_t* p_config );
unsigned int   config_current_line    ( config_t* p_config );

/* TODO: Expose a way to manipulate the settings data structure
 * so that it may be saved to a file stream. The pieces are there
 * but it isn't tested.
 */
bool config_add_group   ( config_t* p_config, const char* name );
bool config_add_setting ( lc_tree_map_t* p_group, const char* name, const lc_variant_t* p_variant );
bool config_add_string  ( lc_tree_map_t* p_group, const char* name, const char* value );
bool config_add_boolean ( lc_tree_map_t* p_group, const char* name, bool value );
bool config_add_integer ( lc_tree_map_t* p_group, const char* name, long value );
bool config_add_decimal ( lc_tree_map_t* p_group, const char* name, double value );


#endif /* _CONFIG_PRIVATE_H_ */
