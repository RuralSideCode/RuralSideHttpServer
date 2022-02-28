#pragma once

#include <map>
#include <string>
#include <vector>

class HttpHeader{
	public:
	
		/**
		 * Default constructor for HttpHeader
		 */
		HttpHeader(); 

		/**
		 * Given a field name, 
		 * this function will search its HttpHeader for a corresponding value
		 * @param fieldName The key value to search for
		 * @return A value that was found within the HttpHeader, or a nullptr if one does not exist
		 */
		std::string getField(std::string fieldName) const;

		/**
		 * This will set a field and value pair within the HttpHeader
		 * @param fieldName The key value
		 * @param fieldValue The value item
		 */
		void setField(std::string fieldName, std::string fieldValue);

		/**
		 * Will gather all the keys that exist within the HttpHeader
		 * @return A const vector<string> object containing all of the key values
		 */
		const std::vector<std::string> getKeys() const;

		const std::string getRequestMethod() const;
		const std::string getRequestResource() const;
		const std::string getRequestVersion() const;
		const std::string getStatusCode() const;

		void setRequestMethod(std::string method);
		void setRequestResource(std::string resource);
		void setRequestVersion(std::string version);
		void setStatusCode(std::string status);

		/**
		 * Returns a string version of the HttpHeader
		 * @return String version of the header
		 */
		std::string str() const;
	private:

		std::map<std::string, std::string> httpHeaderEntries;

		std::string requestMethod;
		std::string requestResource; 
		std::string requestVersion;
		std::string statusCode;
};
