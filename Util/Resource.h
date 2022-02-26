#pragma once

#include <string>

class Resource{
	public:
		Resource() = default;

		~Resource();

		const char* getData() { return data; }
		size_t size() { return dataSize; }

	private:
		char* data = nullptr;
		size_t dataSize = 0;

		std::string dataLocation;

		friend class ResourceLoader;
};

class ResourceLoader{
	public:
		ResourceLoader() = default;
		ResourceLoader(const char* rootLocation) { this->rootLocation = rootLocation; }

		void setRootLocation(const char* rootLocation) { this->rootLocation = rootLocation; } 

		Resource* load(std::string rootRelativePath);

	private:

		std::string rootLocation;

		std::string getResourcePath(std::string relativePath);
	
};
