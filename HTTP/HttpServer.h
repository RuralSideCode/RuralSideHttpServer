#pragma once
#include <string.h>
#include <functional>

#include "Connection.h"

class HttpServer{

	public:
		
		HttpServer() = default;

		int request(ConnectionDetails conn);

	private:

	const char* loadResource(std::string resourceLocation);

};


void httpServerCallback(ConnectionDetails conn, HttpServer* server);

auto createHttpServerCallback(HttpServer* server) -> decltype(std::bind(httpServerCallback, std::placeholders::_1, server));
