#pragma once

#include "HttpHeader.h"

#include <string>

class HttpMessage{

	public:
		/**
		 * Creates a HttpMessage with a header, and data.
		 * @param header The header to use when constructing the message
		 * @param data The raw data that is to be sent with the header
		 * @param dataSize The size in bytes of the raw data
		 */
		HttpMessage(HttpHeader& header, const void* data, const int dataSize);
		~HttpMessage();
		
		/**
		 * Returns the data pointer of the message
		 * @param size The integer to store the data size in
		 * @return A const pointer to the data
		 */
		const char* getMessage(int* size);
		
		/**
		 * Works the same as getData() except returns in a std::string
		 * @return A copy of the HttpMessage
		 */
		std::string getMessage_S() { return this->completedMessage; }

		/**
		 * Returns a pointer to the data that was used to construct the HttpMessage
		 * @return The pointer to the HttpMessage data
		 */
		const char* getData() { return this->data; }

		/**
		 * Returns the size of the HttpMessage data
		 * @return The size of the HttpMessage data
		 */
		int size() { return this->dataSize; }

		/**
		 * @return The HttpHeader that was used to construct the HttpMessage
		 */
		const HttpHeader* getHttpHeader();

	private:
		void createMessage();

		const char* data = nullptr;
		int dataSize = 0;

		struct HttpHeader httpHeader;

		std::string completedMessage = "";

};
