#pragma once

#include "HttpHeader.h"

#include <sstream>

class HttpRequestParser{
	
	public:

		/**
		 * This will parse a HTTP string header into usable data.
		 * @param header String text of the HTTP header
		 * @param data Any data that is extracted will be put in here
		 * @param datasize Size of data will be stored here
		 * @return Will return a HttpHeader pointer
		 */
		static HttpHeader* parse(const char* header, void* data, int* dataSize);

		static HttpHeader* parse(std::stringstream& header, void* data, int* dataSize);

	private:

		static void addFieldToHeader(HttpHeader& header, std::stringstream& ss);
		static void addRequestMethod(HttpHeader& header, std::stringstream& ss);
};

