#ifndef NET_INCLUDED
#define NET_INCLUDED

typedef enum {
	NETWORK_CONNECTED,
	NETWORK_CONNECTING,
	NETWORK_DISCONNECTED,
} network_state_t;

network_state_t network_state;

typedef enum {
	NETWORK_OK,
	NETWORK_FAIL,
	NETWORK_ERROR_INIT,
	NETWORK_ERROR_BIND
} network_return_t;

network_return_t network_join( char* address, int port );
network_return_t network_host( int local_port );

void network_quit();

#define INET_ADDRSTRLEN 16

typedef struct {
	int port;
	char ip[INET_ADDRSTRLEN];
} network_connection_t;

typedef struct {
	network_connection_t* from;
	int size;
	void* data;
} network_packet_t;

typedef enum {
	NETWORK_EVENT_CONNECT,    // data = network_connection_t
	NETWORK_EVENT_DISCONNECT, // data = network_connection_t
	NETWORK_EVENT_PACKET      // data = network_packet_t
} network_event_t;

// return 0 to continue enumerating
// *data is only valid within the callback

typedef int (*network_event_callback_t)( network_event_t, void* data, void* context );

void network_pump( network_event_callback_t, void * );

// valid flag combinations:
//
//   sequenced  = unreliable + sequenced   - delivers instantly and drops late packets
//   unreliable = unreliable + unsequenced - no reliability or sequencing (like udp)
//   reliable   =   reliable + sequenced   - gauranteed delivery and sequencing (like tcp)
//
// channels:
//
//   a main channel is usually good enough because
//   mixing packets with different flags on the same channel is valid
//   and leads to interesting affects
//
//   channels are mainly good for separating sequenced streams
//   a late position update is normally undesired
//   but you would not want your latest position update dropped
//   because a higher sequenced voice packet arrived first
//   thus separate sequenced streams to avoid unwanted drops
//
//   channels could also be used as packet type identifiers
//
// data:
//
//   data passed to network_send is coppied so you are safe to cleanup the data right away

typedef enum {
	NETWORK_UNRELIABLE  = 0,
	NETWORK_RELIABLE    = 2,
	NETWORK_SEQUENCED   = 4
} network_flags_t;

void network_send( void* data, int size, network_flags_t flags, int channel );

#endif // NET_INCLUDED
