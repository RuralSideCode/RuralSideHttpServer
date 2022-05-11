#include "Configuration.h"

#include <fstream>

#include "Logging.h"

ConfigurationManager::ConfigurationManager(std::string configFile){
	this->loadJson(configFile);
}

void ConfigurationManager::loadJson(std::string configFile){
	try {
		std::ifstream fileStream(configFile);
		if(!fileStream.is_open()) throw std::ios_base::failure("Could not open configuration file");

		fileStream.seekg(0, std::ios::end);
		size_t fileSize = fileStream.tellg();
		fileStream.seekg(0, std::ios::beg);

		char* buf = new char[fileSize];

		fileStream.read(buf, fileSize);

		//This could be a security risk with the char*
		configData = nlohmann::json::parse(buf);
		this->currentPath = configFile;
	} 

	catch(nlohmann::json::parse_error &e){
		Log.error("Error parsing JSON Configuration File");
		Log.error(e.what());
	} 

	catch(std::ios_base::failure &e){
		Log.error("Error opening configuration file");
		Log.error(e.what());
	}
}

void ServerConfiguration::setServer(BoundConnection* connection) {
	this->server = connection;
}

void ServerConfiguration::configureServer() {
	if(server == nullptr){
		Log.warning("ServerConfiguration tried to configure a null server. Please set the BoundConnection in the ServerConfiguration object before use!");
	}

	this->server->setPort(this->configData.at("port"));	
}
