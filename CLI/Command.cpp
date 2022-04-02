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

