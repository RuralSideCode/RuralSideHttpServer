#include "Connection.h"

#include <cstring>
#include <string>
#include <iostream>
#include "Logging.h"

#include <errno.h>

//TODO: Fix this to where I can see ip address and wreck havok on the world
std::string convertCDtoString(const ConnectionDetails& cd){
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

	return std::string(s);	
}

Connection::Connection(){
	addressHints->ai_family = AF_UNSPEC;
}

Connection::Connection(const ConnectionDetails& cd){
	//Check to make sure that the socket file descriptor is valid
	if(cd.socketfd <= 0){
		Log.error("A connection was trying to establish itself with an invalid socket file descriptor (SFD)");
		return;
	}

	socketfd = cd.socketfd;
}

Connection::~Connection(){
	this->closeConnection();
}

ConnectionDetails Connection::getConnectionDetails(){
	struct ConnectionDetails cd{
		this->socketfd,
		*this->addressInfo->ai_addr
	};

	return cd;
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
	if(socketfd <= 0){
		std::cout << "Please create a valid socket before trying to connect" << std::endl;
		return 1;
	}

	addressHints = (struct addrinfo*)std::malloc(sizeof(struct addrinfo));

	if(int rc = connect(socketfd, addressInfo->ai_addr, sizeof(struct sockaddr)) == -1){ 
		std::cout << "Could not create a valid connection with the current socket" << std::endl;
		return 2;
	}

	return 0;
}

void Connection::closeConnection(){
	if(socketfd > 0){
		close(socketfd);
	}

	if(this->addressHints != nullptr){
		freeaddrinfo(this->addressHints);
	}
}

int Connection::createSocket(){
	if(int rc = getaddrinfo(address.c_str(), port.c_str(), addressHints, &addressInfo) != 0){
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

bool Connection::isAlive(){
	char pingData[64]{0};
	int sent_bytes = sendData(pingData, sizeof(pingData));	
	return sent_bytes > 0;
}

int Connection::sendData(const void* buffer, int bufferSize){
	int sent_bytes = send(socketfd, buffer, bufferSize, 0);
	return sent_bytes;
}

int Connection::receiveData(void* buffer, int bufferSize){
	int recived_bytes = recv(socketfd, buffer, bufferSize, 0);
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

	if(protocol == PROTOCOL_TCP)
		socketType = SOCK_STREAM;
	else if(protocol == PROTOCOL_UDP)
		socketType = SOCK_DGRAM;
}

int BoundConnection::createSocket(){
	struct addrinfo hints = this->constructAddressHints();

	if(int rc = getaddrinfo(NULL, port.c_str(), &hints, &this->addressInfo) != 0){
		return 1;
	}

	this->socketfd = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if(this->socketfd == -1){
		return 2;
	}

	return 0;
}

int BoundConnection::bindConnection(){
	if(socketfd == -1){
		return 1;
	}

	if(int rc = bind(this->socketfd, addressInfo->ai_addr, sizeof(struct sockaddr)) == -1){
		return 2;
	}

	return 0;
}

void BoundConnection::closeConnection(){
	for(int i = 0; i < max_connections; i++){
		close(connections[i].socketfd);
	}
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
		ConnectionDetails connectionDetails{
			rc, *(struct sockaddr*)incomingConnection
		};

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

