#include "Connection.h"

/** @file AsyncServer.h AsyncServer provides convenient functions to create an asyncronous server. It takes care of a the threading for you!
 */

/**This will take a fully setup BoundConnection and start the BoundConnection::listenToConnection() subroutine on a new thread.
 * A full setup BoundConnection consists of a BoundConnection that has alread called:
 * BoundConnection::createSocket()
 * BoundConnection::bindConnection()
 * @param boundConnection Pointer to the already created an initalized BoundConnection to launch*/
void launchBoundConnection(BoundConnection* boundConnection);
