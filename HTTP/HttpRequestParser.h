#pragma once

#include "HttpHeader.h"

#include <sstream>
#include <string>

class HttpRequestParser{
	
	public:

		/**
		 * This will parse a HTTP string header into usable data.
		 * If the header is empty, then a nullptr will be returned.
		 * @param header String text of the HTTP header
		 * @param data Any data that is extracted will be put in here
		 * @param datasize Size of data will be stored here
		 * @return Will return a HttpHeader pointer. If there was no data then a nullptr will be returned.
		 */
		static HttpHeader* parse(std::string header, void* data, int* dataSize);

	private:

		/**
		 * This will parse a HTTP string stream into usable data
		 * @param header String text of the HTTP header
		 * @param data Any data that is extracted will be put in here
		 * @param datasize Size of data will be stored here
		 * @return Will return a HttpHeader pointer
		 */
		static HttpHeader* parse(std::stringstream& header, void* data, int* dataSize);

		static void addFieldToHeader(HttpHeader& header, std::stringstream& ss);
		static void addRequestMethod(HttpHeader& header, std::stringstream& ss);
};

