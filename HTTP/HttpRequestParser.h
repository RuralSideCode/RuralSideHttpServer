#pragma once

#include "HttpHeader.h"

#include <sstream>

class HttpRequestParser{
	
	public:
		static HttpHeader* parse(const char* header, void* data, int& dataSize);
		static HttpHeader* parse(std::stringstream& header, void* data, int& dataSize);

	private:
};

