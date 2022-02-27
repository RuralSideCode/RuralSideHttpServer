#include "HttpMessage.h"

#include <sstream>
#include <cstring>
#include <iostream>

HttpMessage::HttpMessage(HttpHeader& header, const void* data, const int dataSize){
	this->httpHeader = header;
	this->data = (char*)data;
	this->dataSize = dataSize;
}


HttpMessage::~HttpMessage(){
	if(data != nullptr)
		delete[] data;
}

const char* HttpMessage::createMessage(){
	std::stringstream message;

	//Header Portion
	message << httpHeader.getRequestMethod() << " " << httpHeader.getRequestResource() << " " << httpHeader.getRequestVersion() << '\n';

	const std::vector<std::string> headerKeys = this->httpHeader.getKeys();

	for(int i = 0; i < headerKeys.size(); i++){
		message << headerKeys[i] << ": " <<	httpHeader.getField(headerKeys[i]) << '\n';
	}

	message << '\n';
	
	//Data portion
	message.write(data, dataSize);

	completedMessage = message.str();

	return completedMessage.c_str();
}

const char* HttpMessage::getData(int& size){
	size = this->completedMessage.size();
	return this->completedMessage.c_str();
}
