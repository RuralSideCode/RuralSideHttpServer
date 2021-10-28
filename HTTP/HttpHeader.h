#pragma once

#include <map>
#include <string>
#include <vector>

class HttpHeader{
	public:
	
		HttpHeader(); 

		const char* getField(std::string fieldName) const;
		void setField(std::string fieldName, std::string fieldValue);

		const std::vector<std::string> getKeys() const;

		const std::string getRequestMethod() const;
		const std::string getRequestResource() const;
		const std::string getRequestVersion() const;

		void setRequestMethod(std::string method);
		void setRequestResource(std::string resource);
		void setRequestVersion(std::string version);
	private:

		std::map<std::string, std::string> httpHeaderEntries;

		std::string requestMethod;
		std::string requestResource; 
		std::string requestVersion;
};
