#pragma once

#include <functional>
#include <string>

class Command {

	public:
		Command(): commandName("") {}
		Command(std::string commandName): commandName(commandName) {}

		virtual void invoke() = 0;

		const std::string getCommandName() const{ return commandName; }

	private:
		std::string commandName;
		
};

class ActionCommand : public Command {

	public:
		
		ActionCommand(std::function<void()> command);
		ActionCommand(std::string commandName, std::function<void()> command);

		void invoke();
		
	private:
		std::function<void()> command;

};