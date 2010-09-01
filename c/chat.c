#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net.h"

#define MAX_LINE 100
#define CHAT_CHANNEL 1

char * host_ip;
int host_port = 2300;
int hosting = 0;

int usage( char* str )
{
	if(str != NULL)
		puts(str);
	printf( "Usage: ./chat <host-address> [host-port]\n" );
	return 1;
}

int parse_command_line( int argc, char ** argv )
{
	if ( argc < 2 )
	{
		hosting = 1;
		return 0;
	}
	host_ip = (char*) argv[1];
	if(argv[2])
		host_port = atoi( argv[2] );
	printf("-- command line arguments: ");
	printf("host_ip=%s, host_port=%d\n",
		host_ip, host_port);
	return 0;
}

int setup_network( void )
{
	atexit(network_quit);
	if(hosting)
	{
		network_host(host_port);
		printf("-- waiting for connections.\n");
	}
	else
	{
		network_return_t rv = network_join( host_ip, host_port );
		if( rv != NETWORK_OK )
		{
			puts("-- failed to connect to host");
			puts("-- most likely your arguments were invalid");
			return usage(NULL);
		}
		puts("-- waiting to connect to host.");
		while( network_state == NETWORK_CONNECTING )
			network_pump(NULL,NULL);
		if( network_state == NETWORK_DISCONNECTED )
		{
			puts("-- failed to connect to host to host.");
			exit(1);
		}
		puts("-- successfully connected to host");
	}
	return 0;
}

void send_message( char* line )
{
	network_broadcast((void*) line, strlen(line)+1,
		NETWORK_RELIABLE, CHAT_CHANNEL );
}

int handle_packet( network_event_t type, void* data, void* context )
{
	switch (type)
	{
	case NETWORK_EVENT_CONNECT:
	case NETWORK_EVENT_DISCONNECT:
		{
			network_connection_t* connection = data;
			printf("-- %s from %s:%d\n",
				(type == NETWORK_EVENT_CONNECT) ?
					"connect" : "disconnect",
				connection->ip,
				connection->port
			);
		}
		break;
	case NETWORK_EVENT_PACKET:
		{
			network_packet_t* packet = data;
			char message[packet->size+1];
			strncpy( message, (char*)packet->data, packet->size+1 );
			printf( "%s:%d > %s\n",
				packet->from->ip,
				packet->from->port,
				message 
			);
			if(hosting) // proxy to clients
				send_message( message );
		}
		break;
	}
	return 0;	
}

void check_input( void );
int main( int argc, char ** argv )
{
	setvbuf( stdin, NULL, _IONBF, 0 );

	if(parse_command_line( argc, argv ))
		return 1;

	if(setup_network())
		return 1;

	while( 1 )
	{
		check_input();
		network_pump( handle_packet, NULL );
		network_flush();
		usleep(1000*100); // 100ms, 0.1s
	}

	return 0;
}

////////////////////////////////////////////////////////
// Console handling
////////////////////////////////////////////////////////

int stdin_ready( void )
{
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec  = 0;
	tv.tv_usec = 1; // microsecond

	FD_ZERO(&readfds);
	FD_SET(fileno(stdin), &readfds);

	if ( select( fileno(stdin)+1, &readfds, NULL, NULL, &tv ) > 0 )
	{
		// (FD_ISSET(fileno(stdin), &readfds))
		//{
			return 1;
		//}
	}

	return 0;
}

int read_line( char* line )
{
	int size, c = 0;
	while( stdin_ready() )
	{
		size = strlen(line);
		c = getchar();
		if( c != EOF && c != '\n' && size < (MAX_LINE-1) )
		{
			line[size] = c;
		}
		else
		{
			line[size] = 0;
			return (size==0)?0:size+1;
		}
	}
	return 0; // don't have a full line yet
}

void check_input( void )
{
	static char line[ MAX_LINE ];
	bzero( line, sizeof(line) );
	int size = read_line( line );
	if( size > 0 )
	{
		send_message(line);
		memset( line, 0, size );
	}
}
