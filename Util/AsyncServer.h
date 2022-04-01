#pragma once
#include "Connection.h"

#include <pthread.h>

/** @file AsyncServer.h AsyncServer provides convenient functions to create an asyncronous server. It takes care of a the threading for you!
 */

/**This will take a fully setup BoundConnection and start the BoundConnection::listenToConnection() subroutine on a new thread.
 * A full setup BoundConnection consists of a BoundConnection that has alread called:
 * BoundConnection::createSocket()
 * BoundConnection::bindConnection()
 * @param boundConnection Pointer to the already created an initalized BoundConnection to launch*/
pthread_t launchBoundConnection(BoundConnection* boundConnection);

class AsyncBoundConnection: public BoundConnection{
	public:
		
		pthread_t startConnection();

		pthread_t getCurrentThread() const { return currentThread; }
		
	private:

		pthread_t currentThread;
};
