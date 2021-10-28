#include "HttpHeader.h"

#include <vector>
#include <iostream>

HttpHeader::HttpHeader(){
	httpHeaderEntries = std::map<std::string, std::string>();
}

const char* HttpHeader::getField(std::string fieldName) const{
	return this->httpHeaderEntries.find(fieldName)->second.c_str();
}

void HttpHeader::setField(std::string fieldName, std::string fieldValue){
	this->httpHeaderEntries.insert({fieldName, fieldValue});	
}

const std::vector<std::string> HttpHeader::getKeys() const{
	std::vector<std::string> keys;

	for(auto it = httpHeaderEntries.begin(); it != httpHeaderEntries.end(); it++){
		keys.push_back(it->first);
	}

	return keys;
}

const std::string HttpHeader::getRequestMethod() const{
	return this->requestMethod;
}
const std::string HttpHeader::getRequestResource() const{
	return this->requestResource;
}
const std::string HttpHeader::getRequestVersion() const{
	return this->requestVersion;
}

void HttpHeader::setRequestMethod(std::string method){
	this->requestMethod = method;
}
void HttpHeader::setRequestResource(std::string resource){
	this->requestResource = resource;
}
void HttpHeader::setRequestVersion(std::string version){
	this->requestVersion = version;
}
