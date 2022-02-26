#include "Resource.h"

#include <fstream>

Resource* ResourceLoader::load(std::string rootRelativePath){

	Resource* resource = new Resource;

	std::ifstream in(getResourcePath(rootRelativePath), std::ios::binary);

	size_t fileSize = in.gcount();

	resource->dataSize = fileSize;

	resource->data = new char[fileSize];

	in.read(resource->data, fileSize);

	return resource;
}

std::string ResourceLoader::getResourcePath(std::string relativePath){
	return rootLocation + "\\" + relativePath;	
}

Resource::~Resource(){
	delete[] data;
}
