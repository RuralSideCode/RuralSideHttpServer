#include "HttpServer.h"

#include <iostream>

void httpServerCallback(ConnectionDetails conn, HttpServer* server){
	server->request(conn);
}


auto createHttpServerCallback(HttpServer* server) -> decltype(std::bind(httpServerCallback, std::placeholders::_1, server)){
	return std::bind(httpServerCallback, std::placeholders::_1, server);
}

int HttpServer::request(ConnectionDetails conn){
	std::cout << "Sending data" << std::endl;
	Connection connection(conn);

	connection.sendData("Hello", 5);

	return 5;
}
