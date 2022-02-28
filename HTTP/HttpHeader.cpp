#include "HttpHeader.h"

#include <vector>

#include <sstream>
#include "Logging.h"

HttpHeader::HttpHeader(){
	httpHeaderEntries = std::map<std::string, std::string>();
}

std::string HttpHeader::getField(std::string fieldName) const{
	return this->httpHeaderEntries.find(fieldName)->second;
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
const std::string HttpHeader::getStatusCode() const{
	return this->statusCode;
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
void HttpHeader::setStatusCode(std::string status){
	this->statusCode = status;
}

std::string HttpHeader::str() const{
	std::stringstream result;

	//First line of the header
	if(!requestMethod.empty()){
		result << requestMethod << " ";
	}
	if(!requestResource.empty()){
		result << requestResource << " ";
	}
	if(!requestVersion.empty()){
		result << requestVersion << " ";
	}
	if(!statusCode.empty()){
		result << statusCode << " ";
	}

	result << '\n';

	//Adding the rest of the header
	auto keys = getKeys();

	for(std::string key : keys){
		result << key << ": " << getField(key) << '\n';
	}

	return result.str();
}
