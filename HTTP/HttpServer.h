#pragma once
#include <string.h>
#include <functional>

#include "Connection.h"
#include "HttpHeader.h"
#include "Resource.h"

#define HEADER_BUF_SIZE 1024
#define HEADER_DATA_BUF_SIZE 2048

class HttpServer{

	public:
		
		HttpServer();

		void request(const ConnectionDetails& conn);

		void setResourceLocation(std::string location) { resourceLoader.setRootLocation(location.c_str()); }

	private:

		ResourceLoader resourceLoader;

		const char* loadResource(std::string resourceLocation);

		void handleRequest(HttpHeader& httpHeader, char* data, int dataSize,  Connection& connection);

		void httpGETRequest(HttpHeader& httpHeader, char* data, int dataSize, Connection& connection);
};


void httpServerCallback(const ConnectionDetails& conn, HttpServer* server);

auto createHttpServerCallback(HttpServer* server) -> decltype(std::bind(httpServerCallback, std::placeholders::_1, server));
