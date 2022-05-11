#include <iostream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <memory>
#include <functional>

#include "Connection.h" 
#include "HttpRequestParser.h"
#include "HttpServer.h"
#include "AsyncServer.h"

#include "Logging.h"

#include "Command.h"
#include "Configuration.h"

void shutdownServer(AsyncBoundConnection* connection){
	connection->shutdownConnection();

	pthread_join(connection->getCurrentThread(), NULL);
}

int main(){
	Log.init();
	
	ConsoleLoggingHandler consoleLogger;
	Log.addHandler(&consoleLogger);

	FileOutputLoggingHandler fileLogger;
	fileLogger.setFilePath("../Log.log");
	Log.addHandler(&fileLogger);

	Log << "LOGGER: Logger Initialized!" << Logging::endl;

	//Get current Process id
	pid_t main_pid = getpid();

	//Connection setup
	AsyncBoundConnection asyncBoundConnection;
	ServerConfiguration serverConfiguration;
	serverConfiguration.loadJson("../Configuration/serverConfig.json");
	serverConfiguration.setServer((BoundConnection*)&asyncBoundConnection);
	serverConfiguration.configureServer();

	if(int rc = asyncBoundConnection.createSocket() != BOUNDCONNECTION_OK){
		Log.error("Error creating a socket");
		Log << "RETURN CODE: " << rc << Logging::endl;
		return rc;
	}	

	if(int rc = asyncBoundConnection.bindConnection() != BOUNDCONNECTION_OK){
		Log.error("Could not bind connection");
		Log << "RETURN CODE: " << rc << Logging::endl;
		return rc;
	}

	HttpServer server;
	server.setResourceLocation("../Resources");

	Log.info("Created HTTP server");

	auto serverCallback = createHttpServerCallback(&server);
	asyncBoundConnection.setConnectionCallback(serverCallback);

	Log.info("HTTP server is now connected");

	Log.info("Launching Asyncrounous Server");

	asyncBoundConnection.startConnection();

	ShutdownCommand shutdownServerCommand((BoundConnection*)&asyncBoundConnection);

	while(asyncBoundConnection.isCurrentlyRunning()){
		std::string close;
		std::cout << ">: ";
		std::cin >> close;
		
		if(close.compare(shutdownServerCommand.getCommandName()) == 0){
			shutdownServerCommand.invoke();
			break;
		}

	}
	
	Log.info("Closing Application");
	Log.close();
	return 0;
}
