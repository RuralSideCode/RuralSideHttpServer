#include "Connection.h"

#include <cstring>
#include <string>
#include <iostream>
#include <errno.h>

#include "Logging.h"

ConnectionDetails::ConnectionDetails(SFD_t socketfd, struct sockaddr* address){
	this->socketfd = socketfd;

	if(address != nullptr)
		std::memcpy((void*)&this->address, (const void*)address, sizeof(struct sockaddr));
}

ConnectionDetails::ConnectionDetails(const ConnectionDetails& other){
	this->socketfd = other.socketfd;
	std::memcpy((void*)&this->address, (const void*)&other.address, sizeof(struct sockaddr));
}

//TODO: Fix this to where I can see ip address and wreck havok on the world
std::string convertCDtoString(const ConnectionDetails& cd){
	/*
	char s[INET6_ADDRSTRLEN];
	Log << "FAMILY: " << (int)cd.address.sa_family << Logging::endl;
	Log << "AF_INET: " << (int)AF_INET << Logging::endl;
	Log << "AF_INET6: " << (int)AF_INET6 << Logging::endl;
	Log << "INET6_ADDRSTRLEN: " << (int)INET6_ADDRSTRLEN << Logging::endl;
	Log << "INET_ADDRSTRLEN: " << (int)INET_ADDRSTRLEN << Logging::endl;
	switch(cd.address.sa_family){
		case AF_INET:
				inet_ntop(AF_INET, &(((struct sockaddr_in*)cd.socketfd)->sin_addr), s, sizeof(s));
			break;
		case AF_INET6:
				inet_ntop(AF_INET6, &(((struct sockaddr_in6*)cd.socketfd)->sin6_addr), s, sizeof(s));
			break;
		default:
				//Log.warning("convertCDtoString(ConnectionDetails& cd) just recieved a connection that was not valid");
				inet_ntop(AF_INET, &(((struct sockaddr_in*)cd.socketfd)->sin_addr), s, sizeof(s));
			break;
	}
	*/

	return std::string();	
}

Connection::Connection(){
	addressHints->ai_family = AF_UNSPEC;
}

Connection::Connection(const ConnectionDetails& cd){
	//Check to make sure that the socket file descriptor is valid
	if(cd.getSocketfd() <= 0){
		Log.error("A connection was trying to establish itself with an invalid socket file descriptor (SFD)");
		return;
	}

	connectionDetails = cd;
}

Connection::~Connection(){
	this->closeConnection();
}

ConnectionDetails Connection::getConnectionDetails(){
	return this->connectionDetails;
}

void Connection::setProtocol(Protocol_t protocol){
	addressHints->ai_protocol = protocol;

	if(protocol == PROTOCOL_TCP)
		addressHints->ai_socktype = SOCK_STREAM;
	else
		addressHints->ai_socktype = SOCK_DGRAM;
}

void Connection::setAddress(const char* address){
	this->address = address;
}

void Connection::setSocketType(SocketType_t socketType){
	addressHints->ai_socktype = socketType;
}

void Connection::setPort(const char* _port){
	this->port = _port;
}

void Connection::setPort(int _port){
	this->port = std::to_string(_port);
}

int Connection::createConnection(){
	if(connectionDetails.getSocketfd() <= 0){
		std::cout << "Please create a valid socket before trying to connect" << std::endl;
		return 1;
	}

	addressHints = (struct addrinfo*)std::malloc(sizeof(struct addrinfo)); 

	int socketfd;
	if(int rc = connect(socketfd, addressHints->ai_addr, sizeof(struct sockaddr)) == -1){ 
		std::cout << "Could not create a valid connection with the current socket" << std::endl;
		return 2;
	}

	return 0;
}

void Connection::closeConnection(){
	if(connectionDetails.getSocketfd() > 0){
		close(connectionDetails.getSocketfd());
	}

	if(this->addressHints != nullptr){
		freeaddrinfo(this->addressHints);
	}
}

int Connection::createSocket(){
	struct addrinfo* res;
	if(int rc = getaddrinfo(address.c_str(), port.c_str(), addressHints, &this->addressInfo) != 0){
		std::cout << "Error retreiving address info" << std::endl;
		std::cout << rc << std::endl;
		std::cout << gai_strerror(rc) << std::endl;
		return 1;
	}

	connectionDetails.socketfd = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
	if(connectionDetails.socketfd == -1){
		std::cout << "Error creating socket" << std::endl;
		return 2;
	}

	return 0;
}

// TODO: Implement this
int Connection::ping(){
	char pingData[64]{0};
	int sent_bytes = sendData(pingData, sizeof(pingData));	
	return sent_bytes > 0;
}

int Connection::sendData(const void* buffer, int bufferSize){
	int sent_bytes = send(connectionDetails.socketfd, buffer, bufferSize, 0);
	return sent_bytes;
}

int Connection::receiveData(void* buffer, int bufferSize){
	int recived_bytes = recv(connectionDetails.socketfd, buffer, bufferSize, 0);
	return recived_bytes;
}

BoundConnection::BoundConnection(){
	connections = new ConnectionDetails[max_connections];
}

BoundConnection::~BoundConnection(){
	if(connections)
		delete[] connections;

	freeaddrinfo(this->addressInfo);
}

void BoundConnection::setProtocol(Protocol_t protocol){
	this->protocol = protocol;

	//TODO: This is trash
	if(protocol == PROTOCOL_TCP)
		socketType = SOCK_STREAM;
	else if(protocol == PROTOCOL_UDP)
		socketType = SOCK_DGRAM;
}

int BoundConnection::createSocket(){
	AddressInfo_t hints = this->constructAddressHints();

	if(int rc = getaddrinfo(NULL, port.c_str(), &hints, &this->addressInfo) != 0){
		return BOUNDCONNECTION_NADDRINFO;
	}

	this->socketfd = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if(this->socketfd == -1){
		return BOUNDCONNECTION_NSOCKET;
	}

	return BOUNDCONNECTION_OK;
}

int BoundConnection::bindConnection(){
	if(socketfd == -1){
		return BOUNDCONNECTION_NSOCKET;
	}

	if(int rc = bind(this->socketfd, addressInfo->ai_addr, sizeof(struct sockaddr)) == -1){
		return BOUNDCONNECTION_FAIL_BIND;
	}

	return BOUNDCONNECTION_OK;
}

void BoundConnection::closeConnection(){
	close(this->socketfd);
}

void BoundConnection::setPort(const char* _port){
	this->port = _port;	
}

void BoundConnection::setPort(int _port){
	this->port = std::to_string(_port);
}

void BoundConnection::setConnectionCallback(std::function<void(const ConnectionDetails&)> callback){
	this->callback = callback;
}

struct addrinfo BoundConnection::constructAddressHints(){
	struct addrinfo info;
	std::memset(&info, 0, sizeof(struct addrinfo));
	
	info.ai_family = AF_UNSPEC;
	info.ai_flags = AI_PASSIVE;
	info.ai_protocol = this->protocol;
	info.ai_socktype = this->socketType;

	return info;
}

int BoundConnection::listenToConnection(){
	if(int rc = listen(this->socketfd, this->max_connections) == -1){
		return -1;
	}
	
	pid_t process_id;
	while(isRunning){

		struct sockaddr_storage* incomingConnection;
		socklen_t len = 0;

		int rc = accept(this->socketfd, (struct sockaddr*)incomingConnection, &len);

		if(rc == -1){
			//TODO: Right now we are doing nothing we accept() fails
			return -2;
		}

		//Setting the sockaddr* to the connectionDetails might be causing a memory fault
		ConnectionDetails connectionDetails(rc, (struct sockaddr*) incomingConnection);

		process_id = fork();

		if(process_id == -1){
			return -3;
		}
		else if(process_id == 0){ //If we are the child process
			this->callback(connectionDetails);
			break;
		}
	}

	return process_id;
}
void BoundConnection::shutdownConnection(){
	isRunning = false; //This might cause problems with async server
	this->closeConnection(); //This fixes the problem with Async Server :) (Also should probably call this anyways)
	shutdown(this->socketfd, SHUT_RD);
}

