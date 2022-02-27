#include "HttpServer.h"

#include "Logging.h"

#include "HttpRequestParser.h"
#include "HttpMessage.h"

HttpServer::HttpServer(){
	resourceLoader = ResourceLoader();
}

void httpServerCallback(const ConnectionDetails& conn, HttpServer* server){
	server->request(conn);
}


auto createHttpServerCallback(HttpServer* server) -> decltype(std::bind(httpServerCallback, std::placeholders::_1, server)){
	return std::bind(httpServerCallback, std::placeholders::_1, server);
}

void HttpServer::request(const ConnectionDetails& conn){
	std::string s_address = convertCDtoString(conn);
	Log << Logging::INFO << "Request from " << s_address << Logging::endl;

	Connection connection(conn);

	char httpHeaderBuf[HEADER_BUF_SIZE] {0};

	//We will probably recieve the whole http header in one go, but it is a possibility that we do not
	int recievedBytes = connection.receiveData(httpHeaderBuf, HEADER_BUF_SIZE);

	char httpHeaderData[HEADER_DATA_BUF_SIZE] {0};
	int httpHeaderDataSize = 0; 

	HttpHeader* httpHeader = HttpRequestParser::parse(httpHeaderBuf, (void*)httpHeaderData, &httpHeaderDataSize);

	handleRequest(*httpHeader, httpHeaderData, httpHeaderDataSize, connection); 
}

void HttpServer::handleRequest(HttpHeader& httpHeader, char* data, int dataSize,  Connection& connection){
	std::string requestMethod = httpHeader.getRequestMethod();	

	if(requestMethod == "GET"){
		httpGETRequest(httpHeader, data, dataSize, connection);
	}
}

void HttpServer::httpGETRequest(HttpHeader& httpHeader, char* data, int dataSize, Connection& connection){
	std::string resourceName = httpHeader.getRequestResource();

	if(resourceName == "/"){
		resourceName = "/index.html";
	}

	Resource* resource = resourceLoader.load(resourceName);

	HttpHeader sendHeader;
	sendHeader.setRequestVersion("HTTP/1.1");

	//If the resource was not found give an error 404
	if(resource == nullptr){
		sendHeader.setStatusCode("404");

		HttpMessage httpMessage(sendHeader, nullptr, 0);	

		connection.sendData(httpMessage.getData(), httpMessage.size());
		return; //Return due to 404 error
	}

	sendHeader.setStatusCode("200 OK");

	std::string contentType = "text/" + resourceName.substr(resourceName.find_last_of(".") + 1) + "; charset=UTF-8";
	sendHeader.setField("Content-Type", contentType);
	sendHeader.setField("Content-Length", std::to_string(resource->size()));
	sendHeader.setField("Server", "RuralSideServer/0.1 (Linux)");

	HttpMessage httpMessage(sendHeader, (const void*)resource->getData(), resource->size());

	int sendMessageSize;
	const char* sendMessage = httpMessage.getData(&sendMessageSize);

	connection.sendData(sendMessage, sendMessageSize);
}
