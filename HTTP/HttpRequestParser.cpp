#include "HttpRequestParser.h"

#include <string>
#include <algorithm>

#include <iostream>

HttpHeader* HttpRequestParser::parse(const char* header, void* data, int* dataSize){
	std::stringstream stream(header);

	return HttpRequestParser::parse(stream, data, dataSize);
}

HttpHeader* HttpRequestParser::parse(std::stringstream& header, void* data, int* dataSize){

	std::string line;

	HttpHeader* httpHeader = new HttpHeader;

	std::getline(header, line);
	std::stringstream requestMethodStream(line);

	addRequestMethod(*httpHeader, requestMethodStream); 

	while(std::getline(header, line, '\n')){
		std::stringstream ss(line);
		addFieldToHeader(*httpHeader, ss);
	}

	return httpHeader;
}

void HttpRequestParser::addFieldToHeader(HttpHeader& header, std::stringstream& ss){
	std::string fieldName;
	std::string fieldValue;

	//Get fieldName
	std::getline(ss, fieldName, ':');

	//Get fieldValue
	std::getline(ss, fieldValue);

	//Erase whitespace
	fieldName.erase(remove(fieldName.begin(), fieldName.end(), ' '), fieldName.end());
	fieldValue.erase(remove(fieldValue.begin(), fieldValue.end(), ' '), fieldValue.end());

	//Erase linefeed and carriage returns
	fieldValue.erase(remove(fieldValue.begin(), fieldValue.end(), '\r'), fieldValue.end());
	fieldValue.erase(remove(fieldValue.begin(), fieldValue.end(), '\n'), fieldValue.end());

	header.setField(fieldName.c_str(), fieldValue.c_str());
}

void HttpRequestParser::addRequestMethod(HttpHeader& header, std::stringstream& ss){
	std::string requestMethod;
	std::string requestResource;
	std::string requestVersion;

	std::getline(ss, requestMethod, ' ');
	std::getline(ss, requestResource, ' ');
	std::getline(ss, requestVersion);

	header.setRequestMethod(requestMethod);
	header.setRequestResource(requestResource);
	header.setRequestVersion(requestVersion);
}
