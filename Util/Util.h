#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

/** @file Util.h Util contains lots of definitions used by the server to keeps things more concise*/

typedef int SFD_t; /**< Socket File Descriptor*/

typedef int Protocol_t; /**< Protocol (TCP, UDP, etc.)*/

typedef struct sockaddr Address_t; /**< Socket Address*/

typedef int SocketType_t; /**< Socket Type (SOCK_STREAM, SOCK_DGRAM)
							Note that this will usually be automatically set when setting a protocol(Protocol_t)*/

#define PROTOCOL_TCP IPPROTO_TCP /**< Defines IPPROTO_TCP as PROTOCOL_TCP. This can be used for a Protocol_t*/
#define PROTOCOL_UDP IPPROTO_UDP /**< Defines IPPROTO_UDP as PROTOCOL_UDP. This can be used for a Protocol_t*/
