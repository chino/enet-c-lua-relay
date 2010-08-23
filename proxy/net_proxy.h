#ifndef NET_INCLUDED
#define NET_INCLUDED

// == network state ==

// you can query network_state at any time to check the state of the connection

typedef enum {
	NETWORK_CONNECTED,    // connected to host
	NETWORK_CONNECTING,   // connecting to host
	NETWORK_DISCONNECTED, // not connected yet or lost connection
} network_state_t;

network_state_t network_state;

// == main functions ==

typedef enum {
	NETWORK_OK,         // success
	NETWORK_FAIL,       // failure
	NETWORK_ERROR_INIT, // failed to initialize
	NETWORK_ERROR_BIND  // failed to bind socket
} network_return_t;

network_return_t network_join( char* address, int port );
network_return_t network_host( int local_port );

void network_quit();

typedef struct {
	int size;
	void* data;
} network_packet_t;

typedef void (*network_event_t)( network_packet_t* data );

void network_pump( network_event_t );

// == sending data ==

// unsequenced | unreliable  - is the default
// reliable    | unsequenced - is not supported
// reliable                  - is always sequenced
// sequenced   | unreliable  - will drop late pkts & instantly deliver arrived pkts (position updates)
// sequenced   | reliable    - will wait for late pkts to arive

// multiple types of pkts can all be sent on the same channel
// reliable | sequenced will take part in the same sequencing order on a channel
// unreliable | sequenced pkts will get dropped if they are late...

// channels are mainly good for separating sequenced streams...
// for instance if you have a stream of position updates and another stream for voice pkts
// you would NOT want your position pkt dropped cause a newer voice pkt came in first...
// thus you would create channels to seperate those streams...

typedef enum {
	 NETWORK_RELIABLE  = 2, // note: reliable is also sequenced
	 NETWORK_SEQUENCED = 4
} network_flags_t;

// channel 0 is reserved
// data will be copied so do whatever you want with it afterwards

void network_send( void* data, int size, network_flags_t flags, int channel );

#endif // NET_INCLUDED
