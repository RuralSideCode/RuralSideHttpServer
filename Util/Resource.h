#pragma once

#include <string>
#include <exception>

class Resource{
	public:

		class ResourceException : public std::exception {
			public:
				ResourceException() = default;
				ResourceException(const char* message): message(message) {}

				const char* what() { return this->message; }
			private:
				const char* message = nullptr;
		};

		Resource() = default;

		~Resource();

		const char* getData() { return data; }
		void setData(char* data, size_t size) {
			if (this->data != nullptr) {
				delete[] this->data;
			}

			this->data = data;
			this->dataSize = size;
		}
		void copyData(const char* data, size_t size);

		size_t size() { return dataSize; }

		void save() throw(Resource::ResourceException);

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
