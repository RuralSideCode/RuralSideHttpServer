#include "Logging.h"

#include <sstream>
#include <iostream>
#include <fstream>

bool Logging::isInit = false;
bool Logging::enable = true;

std::vector<LoggingHandler*> Logging::handlers;

void Logging::init() {
	if (isInit) {
		Logging::warning("Logging has already been intialized. Please make sure that the logging module has only called init() once.");
		return;
	}
	isInit = true;
}

void Logging::close() {
	for (int i = 0; i < handlers.size(); i++) {
		if (handlers[i] != nullptr) {
			handlers.erase(handlers.begin() + i);
		}
	}
}

void Logging::addHandler(LoggingHandler* handler) {
	Logging::handlers.push_back(handler);
}

void Logging::log(const char* message, Logging::LogLevel level) {
	for (auto h : handlers) {
		if (h != nullptr)
			h->log(message, level);
	}
}

void Logging::info(const char* message) {
	Logging::log(message, Logging::INFO);
}

void Logging::warning(const char* message) {
	Logging::log(message, Logging::WARNING);
}

void Logging::error(const char* message) {
	Logging::log(message, Logging::ERROR);
}

void Logging::debug(const char* message) {
	Logging::log(message, Logging::DEBUG);
}

void Logging::critical(const char* message) {
	Logging::log(message, Logging::CRITICAL);
}

void Logging::exception(std::exception e) {
	//TODO: This does not display e.what()
	Logging::log(e.what(), Logging::EXCEPTION);
}

const char* Logging::logLevelToString(LogLevel level) {
	switch (level) {
	case LogLevel::DEBUG: return "DEBUG"; break;
	case LogLevel::INFO: return "INFO"; break;
	case LogLevel::WARNING: return "WARNING"; break;
	case LogLevel::ERROR: return "ERROR"; break;
	case LogLevel::CRITICAL: return "CRITICAL"; break;
	case LogLevel::EXCEPTION: return "EXCEPTION"; break;
	default: return ""; break;
	}
}

std::string Logging::getBasicLogMessage(const char* message, Logging::LogLevel level) {
	std::stringstream ss;
	ss << Logging::logLevelToString(level);
	ss << ": ";
	ss << message;
	ss << '\n';
	return ss.str();
}

void LoggingHandler::log(const char* message, Logging::LogLevel messageLevel) {
	if (!this->enable) return;

	if (messageLevel >= this->level) {
		this->logMessage(message, messageLevel);
	}
}

void LoggingHandler::logMessage(const char* message, Logging::LogLevel messageLevel) {}

void ConsoleLoggingHandler::logMessage(const char* message, Logging::LogLevel messageLevel) {
	std::string newMessage = Logging::getBasicLogMessage(message, messageLevel);
	std::cout << newMessage;
}

void FileOutputLoggingHandler::logMessage(const char* message, Logging::LogLevel messageLevel) {

	std::ofstream output(this->filePath, std::ios_base::out | std::ios_base::app);

	if (output.is_open()) {
		std::string loggedMessage = Logging::getBasicLogMessage(message, messageLevel);

		output << loggedMessage;

		output.close();
		return;
	}

	throw LoggingException("Could not log to file set");
}

void FileOutputLoggingHandler::setFilePath(std::string filePath) {
	std::ofstream output(filePath);

	if (output.is_open()) {
		output.close();

		this->filePath = filePath;

		return;
	}

	throw LoggingException("Could not open file in the declared filePath in FileOutputLoggingHandler::setFilePath()");
}

LoggingException::LoggingException(const char* message) {
	this->message = message;
}

const char* LoggingException::what() {
	return message;
}
