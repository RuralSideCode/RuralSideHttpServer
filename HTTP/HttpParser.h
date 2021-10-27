#pragma once

#include <istream>

#include "HttpHeader.h"

class HttpRequestParser{
	
	public:
		static HttpHeader* parse(const char* header, void* data, int& dataSize);

		static HttpHeader* parse(std::istream& header, void* data, int& dataSize);

	private:
};

