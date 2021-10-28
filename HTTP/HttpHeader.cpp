#include "HttpHeader.h"

#include <vector>
#include <iostream>

HttpHeader::HttpHeader(){
	httpHeaderEntries = std::map<std::string, std::string>();
}

const char* HttpHeader::getField(std::string fieldName){
	return this->httpHeaderEntries.find(fieldName)->second.c_str();
}

void HttpHeader::setField(std::string fieldName, std::string fieldValue){
	this->httpHeaderEntries.insert({fieldName, fieldValue});	
}

std::vector<std::string> HttpHeader::getKeys(){
	std::vector<std::string> keys;

	for(auto it = httpHeaderEntries.begin(); it != httpHeaderEntries.end(); it++){
		keys.push_back(it->first);
	}

	return keys;
}
