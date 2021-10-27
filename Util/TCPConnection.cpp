#include "TCPConnection.h"

TCPConnection::TCPConnection(){
	this->addressHints.ai_protocol = PROTOCOL_TCP;
	this->addressHints.ai_socktype = SOCK_STREAM;
}
