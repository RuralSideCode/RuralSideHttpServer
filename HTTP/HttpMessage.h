#pragma once

#include "HttpHeader.h"

#include <string>

class HttpMessage{

	public:
		HttpMessage(HttpHeader& header, void* data, int dataSize);
		~HttpMessage();
		
		const char* createMessage() const;

		const char* getData(int& size);

		const HttpHeader* getHttpHeader();

	private:
		char* data = nullptr;
		int dataSize = 0;

		struct HttpHeader httpHeader;

		char* completedMessage = nullptr;

};
