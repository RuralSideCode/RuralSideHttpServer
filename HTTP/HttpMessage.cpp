#include "HttpMessage.h"

#include<sstream>
#include <cstring>

HttpMessage::HttpMessage(HttpHeader& header, void* data, int dataSize){
	this->httpHeader = header;
	this->data = (char*)data;
	this->dataSize = dataSize;
}


HttpMessage::~HttpMessage(){
	if(data != nullptr)
		delete[] data;
}

const char* HttpMessage::createMessage() const{
	std::stringstream message;

	message << httpHeader.getRequestMethod() << " " << httpHeader.getRequestResource() << " " << httpHeader.getRequestVersion() << '\n';

	const std::vector<std::string> headerKeys = this->httpHeader.getKeys();

	for(int i = 0; i < headerKeys.size(); i++){
		message << headerKeys[i] << ": " <<	httpHeader.getField(headerKeys[i]) << '\n';
	}

	message << '\n';

	std::string _message = message.str();

	if(data != nullptr){
		//This may fuck up bad
		_message += completedMessage;	
	}

	std::strcpy(completedMessage, _message.c_str());

	return completedMessage;
}
