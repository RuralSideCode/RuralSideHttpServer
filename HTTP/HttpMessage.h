#pragma once

#include "HttpHeader.h"

#include <string>

class HttpMessage{

	public:
		HttpMessage();
		~HttpMessage();

		const char* getData(int& size);

		const HttpHeader* getHttpHeader();

	private:
		char* data = nullptr;
		int dataSize = 0;

		struct HttpHeader httpHeader;

};
