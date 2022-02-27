#include <iostream>
#include <vector>

#include "Connection.h" 
#include "HttpRequestParser.h"
#include "HttpServer.h"

int main(){
	BoundConnection bc;
	bc.setPort(80);

	if(int rc = bc.createSocket() != 0){
		std::cout << "Error Creating Socket" << std::endl;
		std::cout << "Return code " << rc << std::endl;
		return 1;
	}	

	if(bc.bindConnection() != 0){
		std::cout << "Error Binding Connection" << std::endl;
		return 2;
	}

	HttpServer server;
	server.setResourceLocation("../Resources");
	std::cout << "Created httpserver" << std::endl;

	auto serverCallback = createHttpServerCallback(&server);
	bc.setConnectionCallback(serverCallback);

	bc.listenToConnection();

	bc.closeConnection();
	
	return 0;
}
