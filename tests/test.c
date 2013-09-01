#include <stdio.h>
#include <libconfig/config.h>


int main( int argc, char* argv[] )
{
	config_t* p_config = config_create( "test.cfg" );


	printf( "Locale: %s", config_get( p_config, "settings.locale" ) );
	printf( "Alpha: %s", config_get( p_config, "settings.alpha" ) );
	printf( "Beta: %s", config_get( p_config, "settings.beta" ) );
	printf( "Host: %s", config_get( p_config, "settings.network.host" ) );
	printf( "Host: %d", config_get( p_config, "settings.network.port" ) );

	config_destroy( &p_config );
	return 0;
}
