#include "Configuration.h"

#include <fstream>

#include "Logging.h"

ConfigurationManager::ConfigurationManager(std::string configFile){
	this->loadJson(configFile);
}

void ConfigurationManager::loadJson(std::string configFile){

	try {
		std::fstream fileStream(configFile, std::ios_base::in | std::ios_base::out);
		if(!fileStream.is_open()) throw std::ios_base::failure("Could not open configuration file");

		configData = nlohmann::json::parse(configFile);
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

ServerConfiguration::ServerConfiguration(BoundConnection* connection) {
	this->server = connection;;
}

ServerConfiguration::ServerConfiguration(BoundConnection* connection, std::string configFile):ConfigurationManager(configFile){
	this->server = connection;

}
void ServerConfiguration::setServer(BoundConnection* connection) {
	this->server = connection;
}

void ServerConfiguration::configureServer() {
	if(server == nullptr){
		Log.warning("ServerConfiguration tried to configure a null server. Please set the BoundConnection in the ServerConfiguration object before use!");
	}

	
}
