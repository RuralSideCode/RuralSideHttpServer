#pragma once

#include "HttpHeader.h"

#include <string>

class HttpMessage{

	public:
		HttpMessage(HttpHeader& header, const void* data, const int dataSize);
		~HttpMessage();
		
		const char* getData(int* size);
		std::string getData_S() { return this->completedMessage; }

		const char* getData() { return this->data; }
		int size() { return this->dataSize; }

		const HttpHeader* getHttpHeader();

	private:
		void createMessage();

		const char* data = nullptr;
		int dataSize = 0;

		struct HttpHeader httpHeader;

		std::string completedMessage = "";

};
