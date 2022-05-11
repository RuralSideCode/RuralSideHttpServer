#include <pthread.h>

#include "Command.h"

ActionCommand::ActionCommand(std::string commandName, std::function<void()> command): Command(commandName) {
	this->command = command;
}

ActionCommand::ActionCommand(std::function<void()> command): Command(""){
	this->command = command;
}

void ActionCommand::invoke() {
	this->command();
}

ShutdownCommand::ShutdownCommand(BoundConnection* connection): Command("Shutdown") {
	this->connection = connection;
}

void ShutdownCommand::invoke(){
	this->connection->shutdownConnection();


}
