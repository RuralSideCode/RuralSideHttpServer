#include "AsyncServer.h"

#include "Logging.h"

void* startBoundConnection(void* boundConnection){
	BoundConnection* connection = (BoundConnection*)boundConnection;

	pid_t thread_pid = getpid();
	pid_t connection_pid = connection->listenToConnection();

	if(connection_pid ==  thread_pid){
		connection->closeConnection();
		pthread_exit(0);
	}

	return nullptr;
}

AsyncBoundConnection::~AsyncBoundConnection(){
	pthread_join(currentThread, NULL);
}

pthread_t launchBoundConnection(BoundConnection* boundConnection){

	 pthread_t thread;

	int rc = pthread_create(&thread, NULL, startBoundConnection, (void*) boundConnection);

	if(rc != 0){
		Log.error("Could not launch Asyncronous Server Bound connection");	
	}

	return thread;
}

void* startAsyncBoundConnection(void* abc){
	AsyncBoundConnection* connection = (AsyncBoundConnection*)abc;

	pid_t thread_pid = getpid();
	pid_t connection_pid = connection->listenToConnection();

	if(connection_pid == thread_pid){
		connection->closeConnection();
		pthread_exit(0);
	}

	return nullptr;
}

pthread_t AsyncBoundConnection::startConnection(){

	int rc = pthread_create(&this->currentThread, NULL, startAsyncBoundConnection, (void*) this);

	if(rc != 0){
		Log.error("Could not launch Asyncronous Server Bound connection");	
	}

	return this->currentThread;
}
