#include <stdio.h>
#include <assert.h>
#if 0
#include <libconfig/config.h>
#else
#include "../src/config.h"
#endif


int main( int argc, char* argv[] )
{
	config_t* p_config = config_create( "/home/joe/projects/libconfig/tests/test.cfg", FALSE );

	lc_variant_t* value = NULL;

	printf( "%s %s %s %s %s %s\n",
			variant_string(config_find(p_config, "message.english")),
			variant_string(config_find(p_config, "message.spanish")),
			variant_string(config_find(p_config, "message.french")),
			variant_string(config_find(p_config, "message.german")),
			variant_string(config_find(p_config, "message.chinese")),
			variant_string(config_find(p_config, "message.russian"))
	);
	printf( "-----------------------------------------\n" );
	value = config_find( p_config, "settings.locale" );
	printf( "Locale: %s\n",  value ? variant_string(value) : "NULL" );
	value = config_find( p_config, "settings.alpha" );
	printf( "Alpha: %lf\n", value ? variant_decimal(value) : 0.0f );
	value = config_find( p_config, "settings.beta" );
	printf( "Beta: %lf\n", value ? variant_decimal(value) : 0.0f );
	value = config_find( p_config, "settings.network.host" );
	printf( "Host: %s\n", value ? variant_string(value) : "NULL" );
	value = config_find( p_config, "settings.network.port" );
	printf( "Port: %ld\n", value ? variant_integer(value) : 0L );
	value = config_find( p_config, "debug.enable" );
	printf( "Debug Enabled: %s\n", value ? (variant_unsigned_integer(value) == 0 ? "False" : "True") : "Unset" );
	printf( "-----------------------------------------\n" );

	config_destroy( &p_config );
	return 0;
}
