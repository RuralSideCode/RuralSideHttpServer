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

		/**
		 * Loads a resource and stores it into the form of Resource
		 * @param path Path for the resource. This is relative to the rootLocation set.
		 * @return A pointer to the resource in the form of Resource. Returns null on failure to load the resource
		 */
		Resource* load(std::string path);

	private:

		std::string rootLocation;

		std::string getResourcePath(std::string relativePath);
	
};
