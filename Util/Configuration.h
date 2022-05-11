#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Connection.h"

/**
 * ConfigurationManager is used to load configuration data
 * and provide an easy interface to retrieve configuration
 * data. The JSON format is used to store and load config data
 */
class ConfigurationManager {
	public:
		ConfigurationManager() = default;
		ConfigurationManager(std::string configFile);

		void loadJson(std::string configFile);
		void saveJson(std::string configFile);
		void saveJson();

	private:
		std::string currentPath;

	protected:
		nlohmann::json configData;
};

class ServerConfiguration : public ConfigurationManager {
	public:
		ServerConfiguration() = default;

		void setServer(BoundConnection* connection);
		void configureServer();

	private:
		BoundConnection* server = nullptr;
};
