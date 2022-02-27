#pragma once

#include "HttpHeader.h"

#include <string>

class HttpMessage{

	public:
		HttpMessage(HttpHeader& header, const void* data, const int dataSize);
		~HttpMessage();
		
		const char* createMessage();

		const char* getData(int& size);
		std::string getData() { return this->completedMessage; }

		const HttpHeader* getHttpHeader();

	private:
		const char* data = nullptr;
		int dataSize = 0;

		struct HttpHeader httpHeader;

		std::string completedMessage = "";

};
