#include "Connection.h"

#include <cstring>
#include <string>
#include <iostream>

Connection::Connection(){
	std::memset(&addressHints, 0, sizeof(struct addrinfo));
	addressHints.ai_family = AF_UNSPEC;
}

Connection::~Connection(){
	freeaddrinfo(this->addressInfo);
}

void Connection::setProtocol(Protocol_t protocol){
	addressHints.ai_protocol = protocol;

	if(protocol == PROTOCOL_TCP)
		addressHints.ai_socktype = SOCK_STREAM;
	else
		addressHints.ai_socktype = SOCK_DGRAM;
}

void Connection::setAddress(const char* address){
	this->address = address;
}

void Connection::setSocketType(SocketType_t socketType){
	addressHints.ai_socktype = socketType;
}

void Connection::setPort(const char* _port){
	this->port = _port;
}

void Connection::setPort(int _port){
	std::string s = "";
	s = std::to_string(_port);

	this->setPort(s.c_str());
}

int Connection::createConnection(){
	if(socketfd <= 0){
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
	if(socketfd <= 0) return;
	close(socketfd);
}

int Connection::createSocket(){
	if(int rc = getaddrinfo(address, port, &addressHints, &addressInfo) != 0){
		std::cout << "Error retreiving address info" << std::endl;
		std::cout << rc << std::endl;
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

int Connection::sendData(const void* buffer, int bufferSize){
	int sent_bytes = send(socketfd, buffer, bufferSize, NULL);
	return sent_bytes;
}

int Connection::receiveData(void* buffer, int bufferSize){
	int recived_bytes = recv(socketfd, buffer, bufferSize, NULL);
	return recived_bytes;
}

BoundConnection::BoundConnection(){
	connections = new ConnectionDetails[max_connections];
}

BoundConnection::~BoundConnection(){
	if(connections)
		delete[] connections;
}

int BoundConnection::sendData(const void* buffer, int bufferSize){
	
}
int BoundConnection::receiveData(void* buffer, int bufferSize){
}

int BoundConnection::createSocket(){
}
int BoundConnection::bindBoundConnection(){
	if(int rc = bind(this->socketfd, addressInfo->ai_addr, sizeof(struct sockaddr)) == -1){
		std::cout << "Error binding BoundConnection" << std::endl;
		return rc;
	}

	return 0;
}

void BoundConnection::closeConnection(){
	for(int i = 0; i < max_connections; i++){
		close(connections[i].socketfd);
	}
}


