#include "HttpRequestParser.h"

#include <string>

HttpHeader* HttpRequestParser::parse(const char* header, void* data, int& dataSize){
	std::stringstream stream(header);

	return HttpRequestParser::parse(stream, data, dataSize);
}

HttpHeader* HttpRequestParser::parse(std::stringstream& header, void* data, int& dataSize){
	return nullptr;
}

