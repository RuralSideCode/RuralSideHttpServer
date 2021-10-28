#include <iostream>
#include <vector>

#include "Connection.h" 
#include "HttpRequestParser.h"

void callback(ConnectionDetails cd){
	Connection conn(cd);

	char buf[1024];
	conn.receiveData(buf, 1024);

	HttpHeader header = *HttpRequestParser::parse(buf, nullptr, nullptr);

	if(header.getRequestMethod() == "GET" && header.getRequestResource() == "/"){
		std::cout << "Send out the HTML file!" << std::endl;
	}

	conn.closeConnection();
}


int main(){

	BoundConnection bc;
	bc.setPort(80);
	bc.setConnectionCallback(callback);

	if(int rc = bc.createSocket() != 0){
		std::cout << "Error Creating Socket" << std::endl;
		std::cout << "Return code " << rc << std::endl;
		return 1;
	}	

	if(bc.bindConnection() != 0){
		std::cout << "Error Binding Connection" << std::endl;
		return 2;
	}

	bc.listenToConnection();

	bc.closeConnection();
	
	return 0;
}
