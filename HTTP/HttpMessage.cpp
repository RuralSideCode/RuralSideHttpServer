#include "HttpMessage.h"

#include <sstream>
#include <cstring>
#include <iostream>

HttpMessage::HttpMessage(HttpHeader& header, const void* data, const int dataSize){
	this->httpHeader = header;
	this->data = (char*)data;
	this->dataSize = dataSize;

	this->createMessage();
}


HttpMessage::~HttpMessage(){
	if(data != nullptr)
		delete[] data;
}

void HttpMessage::createMessage(){
	std::stringstream message;

	message << httpHeader.str();

	message << '\n';
	
	//Data portion
	if(data != nullptr){
		message.write(data, dataSize);
	}

	completedMessage = message.str();
}

const char* HttpMessage::getMessage(int* size){
	*size = this->completedMessage.size();
	return this->completedMessage.c_str();
}
