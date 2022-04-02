#include <iostream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <memory>

#include "Connection.h" 
#include "HttpRequestParser.h"
#include "HttpServer.h"
#include "AsyncServer.h"

#include "Logging.h"

int main(){
	Log.init();
	
	ConsoleLoggingHandler consoleLogger;
	Log.addHandler(&consoleLogger);

	FileOutputLoggingHandler fileLogger;
	fileLogger.setFilePath("../Log.log");
	Log.addHandler(&fileLogger);

	Log << "Logger Initialized!" << Logging::endl;

	//Get current Process id
	pid_t main_pid = getpid();

	//Connection setup
	AsyncBoundConnection asyncBoundConnection;
	asyncBoundConnection.setPort(80);

	Log.info("Created a Bound Connection listening to port 80");

	if(int rc = asyncBoundConnection.createSocket() != 0){
		Log.error("Error creating a socket");
		return rc;
	}	

	if(int rc = asyncBoundConnection.bindConnection() != 0){
		Log.error("Could not bind connection");
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

	while(asyncBoundConnection.isCurrentlyRunning()){
		char close;
		std::cin >> close;
		if(close == 'q'){
			asyncBoundConnection.shutdown();
		}
	}
	
	Log.info("Closing Application");
	Log.close();
	return 0;
}
