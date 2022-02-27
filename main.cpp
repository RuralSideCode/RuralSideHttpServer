#include <iostream>
#include <vector>

#include "Connection.h" 
#include "HttpRequestParser.h"
#include "HttpServer.h"

#include "Logging.h"

int main(){
	Log.init();
	
	ConsoleLoggingHandler consoleLogger;
	Log.addHandler(&consoleLogger);

	FileOutputLoggingHandler fileLogger;
	fileLogger.setFilePath("../Log.log");
	Log.addHandler(&fileLogger);

	Log << "Logger Initialized!" << Logging::endl;

	//Connection setup
	BoundConnection bc;
	bc.setPort(80);

	Log.info("Created a Bound Connection listening to port 80");

	if(int rc = bc.createSocket() != 0){
		Log.error("Error creating a socket");
		return rc;
	}	

	if(int rc = bc.bindConnection() != 0){
		Log.error("Could not bind connection");
		return rc;
	}

	HttpServer server;
	server.setResourceLocation("../Resources");

	Log.info("Created HTTP server");

	auto serverCallback = createHttpServerCallback(&server);
	bc.setConnectionCallback(serverCallback);

	Log.info("HTTP server is now connected to our connection");

	bc.listenToConnection();

	Log.info("Closing connection");

	bc.closeConnection();
	
	Log.info("Closing program");
	Log.close();
	return 0;
}
