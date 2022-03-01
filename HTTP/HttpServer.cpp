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
	//std::string s_address = convertCDtoString(conn);
	//Log << Logging::INFO << "Request from " << s_address << Logging::endl;
	
	//Log << Logging::INFO << "New HTTP Request" << Logging::endl;

	Connection connection(conn);

	bool shouldStayOpen = false;

	do{
		char httpHeaderBuf[HEADER_BUF_SIZE] {0};

		//We will probably recieve the whole http header in one go, but it is a possibility that we do not
		int recievedBytes = connection.receiveData(httpHeaderBuf, HEADER_BUF_SIZE);

		char httpHeaderData[HEADER_DATA_BUF_SIZE] {0};
		int httpHeaderDataSize = 0; 

		HttpHeader* httpHeader = HttpRequestParser::parse(httpHeaderBuf, (void*)httpHeaderData, &httpHeaderDataSize);
		
		//If the header is null then the connection should end
		if(httpHeader == nullptr){
			break;
		}

#ifdef LOG_HEADERS
		//Log << "HTTP HEADER: \n" << httpHeader->str() << Logging::endl << Logging::endl;
#endif

		//If the http header we recieved wants to keep the connection alive, then we will
		shouldStayOpen = httpHeader->getField("Connection").compare("keep-alive") == 0;

		handleRequest(*httpHeader, httpHeaderData, httpHeaderDataSize, connection); 
	} while(shouldStayOpen);

	//Log.info("Ending Http Request Session");
	connection.closeConnection();
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

	sendHeader.setStatusCode("200");

	setupHttpHeader(&sendHeader);

	std::string contentType = "text/" + resourceName.substr(resourceName.find_last_of(".") + 1) + "; charset=UTF-8";
	sendHeader.setField("Content-Type", contentType);
	sendHeader.setField("Content-Length", std::to_string(resource->size()));
	sendHeader.setField("Connection", "keep-alive");

	HttpMessage httpMessage(sendHeader, (const void*)resource->getData(), resource->size());

	int sendMessageSize;
	const char* sendMessage = httpMessage.getMessage(&sendMessageSize);

	connection.sendData(sendMessage, sendMessageSize);
}

void setupHttpHeader(HttpHeader* header){
	header->setField("Server", "RuralSideServer/0.1 (Linux");
	header->setField("Host", "ruralsidecode.com");
}
