#include "net.h"
#include "enet/enet.h"
#include <stdio.h>
#include <string.h>

static int initialized = 0;

static ENetHost* enet_host = NULL;
static ENetPeer* host_peer = NULL;

static int max_peers = 40;
static int max_channels = 50;

#define INET_ADDRSTRLEN 16

static char* address_to_str( ENetAddress * address )
{
	char temp[INET_ADDRSTRLEN+10] = "";
	static char ip[INET_ADDRSTRLEN+10] = "";
	enet_address_get_host_ip( address, temp, INET_ADDRSTRLEN );
	sprintf( ip, "%s:%d", temp, address->port );
	return ip;
}

static network_return_t enet_setup( int port )
{
	network_quit();

	if ( ! initialized )
	{
		if (enet_initialize() != 0)
		{
			return NETWORK_ERROR_INIT;
		}
		puts("enet initialized");
		initialized = 1;
	}
	
	if(port)
	{
		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = port;
		enet_host = enet_host_create( &address, max_peers, 0, 0 );
		printf("enet binding to %s\n", address_to_str(&address));
	}
	else
	{
		enet_host = enet_host_create( NULL, max_peers, 0, 0 );
	}

	if ( enet_host == NULL )
	{
		puts("enet host creation failed");
		return NETWORK_ERROR_BIND;
	}
	else
	{
		puts("enet host created");
		return NETWORK_OK;
	}
}

static enet_uint32 convert_flags( network_flags_t flags )
{
	// our default is unreliable and unsequenced
	enet_uint32 enet_flags = ENET_PACKET_FLAG_UNSEQUENCED;
	if ( flags )
	{
		// sequenced and unreliable
		if ( flags & NETWORK_SEQUENCED )
			enet_flags = 0;
		// note: enet reliable is always sequenced
		if ( flags & NETWORK_RELIABLE  )
			enet_flags |= ENET_PACKET_FLAG_RELIABLE;
	}
	return enet_flags;
}

/*
 *
 *  net.h interface
 *
 */

network_return_t network_host( int port )
{
	enet_setup(port);
	if( enet_host == NULL ) return NETWORK_FAIL;
	network_state = NETWORK_CONNECTED;
	return NETWORK_OK;
}

network_return_t network_join( char* str_address, int port )
{
	ENetAddress address;

	if( enet_setup(0) != NETWORK_OK )
		return NETWORK_FAIL;

	network_state = NETWORK_CONNECTING;

	enet_address_set_host( &address, str_address ); 
	address.port = port;

	printf("enet connecting to %s\n",
		address_to_str(&address));

	host_peer = enet_host_connect( enet_host, &address, max_channels );

	if(!host_peer)
		return NETWORK_FAIL;

	return NETWORK_OK;
}

void network_quit( void )
{
	if( enet_host == NULL ) return;
	printf("network: cleanup\n");
	enet_host_flush(enet_host); // send any pending packets
	enet_host_destroy( enet_host );
	enet_host = NULL;
	enet_deinitialize();
	host_peer = NULL;
	initialized = 0;
	network_state = NETWORK_DISCONNECTED;
}

void network_broadcast( void* data, int size, network_flags_t flags, int channel )
{
	return network_send(NULL,data,size,flags,channel);
}

void network_send( network_connection_t* connection, void* data, int size, network_flags_t flags, int channel )
{
	ENetPacket * packet;
	if( ! enet_host || ! data) return;
	packet = enet_packet_create( data, size, convert_flags( flags ) );
	if(!packet) return;
	if(connection)
		enet_peer_send( (ENetPeer*)connection->ptr, channel, packet );
	else
		enet_host_broadcast( enet_host, channel, packet );
}

void network_flush( void )
{
	if(enet_host)
		enet_host_flush( enet_host );
}

void network_disconnect( network_connection_t * connection )
{
	if(enet_host && connection)
		enet_peer_disconnect_now( (ENetPeer*) connection->ptr, 0 );
}

void network_pump( network_event_callback_t handler, void * context )
{
	ENetEvent event;
	if( ! enet_host )
		return;
	while( enet_host_service( enet_host, &event, 0 ) > 0 )
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		case ENET_EVENT_TYPE_DISCONNECT:
			if(event.peer == host_peer)
			{
				if(event.type == ENET_EVENT_TYPE_CONNECT)
					network_state = NETWORK_CONNECTED;
				else
					network_state = NETWORK_DISCONNECTED;
			}
			if(handler)
			{
				network_event_t type = 
					(event.type == ENET_EVENT_TYPE_CONNECT) ? 
						NETWORK_EVENT_CONNECT: 
						NETWORK_EVENT_DISCONNECT;
				network_connection_t connection;
				connection.ptr = event.peer;
				connection.port = event.peer->address.port;
				enet_address_get_host_ip(
					&event.peer->address,
					connection.ip,
					sizeof(connection.ip)
				);
				handler( type, &connection, context );
			}
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			if(handler)
			{
				network_packet_t packet;
				network_connection_t connection;
				connection.ptr = event.peer;
				connection.port = event.peer->address.port;
				enet_address_get_host_ip(
					&event.peer->address,
					connection.ip,
					sizeof(connection.ip)
				);
				packet.from = &connection;
				packet.size = (int) event.packet->dataLength;
				packet.data = (void*) event.packet->data;
				if(handler( NETWORK_EVENT_PACKET, &packet, context ))
					break;
			}
			enet_packet_destroy( event.packet );
			break;
		default:
			break;
		}
	}
}
