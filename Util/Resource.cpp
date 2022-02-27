#include "Resource.h"

#include <fstream>
#include <iostream>

Resource* ResourceLoader::load(std::string rootRelativePath){

	Resource* resource = new Resource;

	std::cout << getResourcePath(rootRelativePath) << std::endl;

	std::ifstream in(getResourcePath(rootRelativePath), std::ios::binary);

	if(in.is_open()){

		in.seekg(0, std::ios::end);
		size_t fileSize = in.tellg();
		in.seekg(0, std::ios::beg);

		resource->dataSize = fileSize;

		resource->data = new char[fileSize];

		in.read(resource->data, fileSize);

		return resource;
	}
	
	return nullptr;
}

std::string ResourceLoader::getResourcePath(std::string relativePath){
	return rootLocation + "/" + relativePath;	
}

Resource::~Resource(){
	delete[] data;
}
