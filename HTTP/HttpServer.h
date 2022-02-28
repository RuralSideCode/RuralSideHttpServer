#pragma once
#include <string.h>
#include <functional>

#include "Connection.h"
#include "HttpHeader.h"
#include "Resource.h"

#define HEADER_BUF_SIZE 1024
#define HEADER_DATA_BUF_SIZE 2048

//#define LOG_HEADERS

class HttpServer{

	public:
		
		HttpServer();

		/**
		 * Callback for a Http Reqeust
		 * @param conn The connection that has an HTTP request
		 */
		void request(const ConnectionDetails& conn);

		/**
		 * Sets the overall root resource location for the HTTP server. Any resource requests from a connection will look in this path for resources to be loaded
		 * @param location The path of the root resource location
		 */
		void setResourceLocation(std::string location) { resourceLoader.setRootLocation(location.c_str()); }

	private:

		ResourceLoader resourceLoader;

		const char* loadResource(std::string resourceLocation);

		void handleRequest(HttpHeader& httpHeader, char* data, int dataSize,  Connection& connection);

		void httpGETRequest(HttpHeader& httpHeader, char* data, int dataSize, Connection& connection);
};


/**
 * This function should not be used by anyone. It is only used to construct a callback for HttpServer.
 */
void httpServerCallback(const ConnectionDetails& conn, HttpServer* server);

/**
 * Creates a server callback to be passed to a BoundConnection
 * @param server The HttpServer object to create a callback function for
 * @return Returns the callback function for the HTTP Server
 */
auto createHttpServerCallback(HttpServer* server) -> decltype(std::bind(httpServerCallback, std::placeholders::_1, server));
