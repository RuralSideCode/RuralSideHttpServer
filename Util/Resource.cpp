#include "Resource.h"

#include <fstream>
#include <iostream>
#include <cstring>

#include "Logging.h"

Resource* ResourceLoader::load(std::string rootRelativePath){

	std::string resourcePath = getResourcePath(rootRelativePath);

	std::ifstream in(resourcePath, std::ios::binary);

	if (!in.is_open()) return nullptr; 

	in.seekg(0, std::ios::end);
	size_t fileSize = in.tellg();
	in.seekg(0, std::ios::beg);

	Resource* resource = new Resource;

	resource->dataSize = fileSize;
	resource->data = new char[fileSize];
	resource->dataLocation = resourcePath;

	in.read(resource->data, fileSize);

	return resource;
}

std::string ResourceLoader::getResourcePath(std::string relativePath){
	return rootLocation + relativePath;	
}

Resource::~Resource(){
	delete[] data;
}

void Resource::copyData(const char* data, size_t size) {
	if (this->data != nullptr) {
		delete[] this->data;
	}

	std::memcpy(this->data, data, size);
	this->dataSize = size;
}

void Resource::save() throw(Resource::ResourceException) {
	std::ofstream stream(this->dataLocation, std::ios_base::out | std::ios_base::trunc);

	if (!stream.is_open()) {
		throw(Resource::ResourceException("Could not save file because ofstream could not be opened"));
	}

	stream.write(this->data, this->dataSize);
}
