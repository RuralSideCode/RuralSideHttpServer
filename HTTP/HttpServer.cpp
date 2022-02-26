#include "HttpServer.h"

void httpServerCallback(ConnectionDetails conn, HttpServer* server){
	server->request(conn);
}


auto createHttpServerCallback(HttpServer* server) -> decltype(std::bind(httpServerCallback, std::placeholders::_1, server)){
	return std::bind(httpServerCallback, std::placeholders::_1, server);
}

int HttpServer::request(ConnectionDetails conn){
	Connection connection(conn);

	connection.sendData("Hello", 5);

	return 5;
}
