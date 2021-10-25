#include "Connection.h"

#include <cstring>
#include <string>
#include <iostream>

Connection::Connection(){
	std::memset(&addressHints, 0, sizeof(struct addrinfo));
}

void Connection::setProtocol(Protocol_t protocol){
	addressHints.ai_protocol = protocol;
}

void Connection::setAddress(Address_t address){
	Address_t* addr = new Address_t;
	*addr = address;

	addressHints.ai_addr = addr;
}

void Connection::setSocketType(SocketType_t socketType){
	addressHints.ai_socktype = socketType;
}

void Connection::setPort(const char* _port){
	delete this->port;
	this->port = _port;
}
void Connection::setPort(int _port){
	std::string s = "";
	s = std::to_string(_port);

	this->setPort(s.c_str());
}

int Connection::createConnection(){
	if(socketfd != 0){
		std::cout << "Please create a valid socket before trying to connect" << std::endl;
		return 1;
	}

	if(int rc = connect(socketfd, addressInfo->ai_addr, sizeof(struct sockaddr)) == -1){ 
		std::cout << "Could not create a valid connection with the current socket" << std::endl;
		return 2;
	}

	return 0;
}

void Connection::closeConnection(){
	close(socketfd);
}

int Connection::createSocket(){
	char node[INET6_ADDRSTRLEN];

	inet_ntop(addressHints.ai_family, addressHints.ai_addr, node, INET6_ADDRSTRLEN);

	if(int rc = getaddrinfo(node, port, &addressHints, &addressInfo) == -1){
		std::cout << "Error retreiving address info" << std::endl;
		std::cout << gai_strerror(rc) << std::endl;
		return 1;
	}

	socketfd = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
	if(socketfd == -1){
		std::cout << "Error creating socket" << std::endl;
		return 2;
	}

	return 0;
}

const struct addrinfo Connection::getAddressInfo(){
	return *addressInfo;
}
