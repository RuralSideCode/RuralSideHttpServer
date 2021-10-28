#pragma once

#include <map>
#include <string>
#include <vector>

class HttpHeader{
	public:
	
		HttpHeader(); 

		const char* getField(std::string fieldName);
		void setField(std::string fieldName, std::string fieldValue);

		std::vector<std::string> getKeys();

		std::string requestMethod;
		std::string requestResource; 
		std::string requestVersion;

	private:

		std::map<std::string, std::string> httpHeaderEntries;
};
