#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>


//Socket File Descriptor
typedef int SFD_t;

typedef int Protocol_t;

typedef struct sockaddr Address_t;

typedef int SocketType_t;

Address_t createAddress(sa_family_t family, char data[14]);
