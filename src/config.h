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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <libcollections/types.h>
#include <libcollections/variant.h>
#include <libcollections/tree-map.h>


struct config;
typedef struct config config_t;


config_t*     config_create                ( const char* filename, boolean verbose );
void          config_destroy               ( config_t** p_config );
lc_variant_t* config_find                  ( const config_t* p_config, const char* key );
const char*   config_find_string           ( const config_t* p_config, const char* key, const char* default_value );
long          config_find_integer          ( const config_t* p_config, const char* key, long default_value );
unsigned long config_find_unsigned_integer ( const config_t* p_config, const char* key, unsigned long default_value );
double        config_find_decimal          ( const config_t* p_config, const char* key, double default_value );
boolean       config_find_boolean          ( const config_t* p_config, const char* key, boolean default_value );
void          config_dump                  ( FILE* file, const config_t* p_config );
boolean       config_is_verbose            ( const config_t* p_config );
void          config_set_verbose           ( config_t* p_config, boolean v );

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define config_get( p_config, key )  _Generic( variant_type(config_find(p_config, key)), \
	VARIANT_STRING:           variant_value(config_find(p_config, key)).string,  \
	VARIANT_DECIMAL:          variant_value(config_find(p_config, key)).decimal,  \
	VARIANT_INTEGER:          variant_value(config_find(p_config, key)).integer,  \
	VARIANT_UNSIGNED_INTEGER: variant_value(config_find(p_config, key)).unsigned_integer,  \
	VARIANT_POINTER:          variant_value(config_find(p_config, key)).pointer,  \
	VARIANT_NOT_INITIALIZED: NULL,\
	default: 0 \
	) (p_config, key)
#endif

#endif /* _CONFIG_H_ */
