#include <stdio.h>
#if 0
#include <libconfig/config.h>
#else
#include "../src/config.h"
#endif


int main( int argc, char* argv[] )
{
	config_t* p_config = config_create( "/home/joe/projects/libconfig/tests/test.cfg" );


	#if 0
	printf( "Locale: %s", variant_string( config_find( p_config, "settings.locale" ) ) );
	printf( "Alpha: %lf", variant_decimal( config_find( p_config, "settings.alpha" ) ) );
	printf( "Beta: %lf", variant_decimal( config_find( p_config, "settings.beta" ) ) );
	printf( "Host: %s", variant_string( config_find( p_config, "settings.network.host" ) ) );
	printf( "Host: %ld", variant_integer( config_find( p_config, "settings.network.port" ) ) );
	#endif

	config_destroy( &p_config );
	return 0;
}
