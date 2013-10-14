#include <stdio.h>
#include <assert.h>
#if 0
#include <libconfig/config.h>
#else
#include "../src/config.h"
#endif

int main( int argc, char* argv[] )
{
	config_t* p_config = config_create( "./tests/test.cfg", FALSE );


	printf( "%s %s %s %s %s %s\n",
			config_find_string(p_config, "message.english", "<uninitialized>"),
			config_find_string(p_config, "message.spanish", "<uninitialized>"),
			config_find_string(p_config, "message.french", "<uninitialized>"),
			config_find_string(p_config, "message.german", "<uninitialized>"),
			config_find_string(p_config, "message.chinese", "<uninitialized>"),
			config_find_string(p_config, "message.russian", "<uninitialized>")
	);
	printf( "-----------------------------------------\n" );
	printf( "Locale: %s\n",  config_find_string(p_config, "settings.locale", "<uninitialized>") );
	printf( "Alpha: %lf\n",  config_find_decimal(p_config, "settings.alpha", 100.0f) );
	printf( "Beta: %lf\n",   config_find_decimal(p_config, "settings.beta", 200.0f) );
	printf( "Host: %s\n",   config_find_string(p_config, "settings.network.host", "<uninitialized>") );
	printf( "Port: %ld\n",   config_find_integer(p_config, "settings.network.port", 0L) );
	printf( "Red: #%06x\n",   (unsigned int) config_find_unsigned_integer(p_config, "settings.red", 0) );
	printf( "Green: #%06x\n",   (unsigned int) config_find_unsigned_integer(p_config, "settings.green", 0) );
	printf( "Blue: #%06x\n",   (unsigned int) config_find_unsigned_integer(p_config, "settings.blue", 0) );
	printf( "Cyan + Alpha: #%08x\n",   (unsigned int) config_find_unsigned_integer(p_config, "settings.cyan_with_alpha", 0) );
	printf( "Debug Enabled: %s\n",  config_find_boolean(p_config, "debug.enable", false) ? "True" : "False" );
	printf( "-----------------------------------------\n" );

	config_destroy( &p_config );
	return 0;
}
