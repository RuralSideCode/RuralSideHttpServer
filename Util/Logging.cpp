#include "Logging.h"

#include <sstream>
#include <iostream>
#include <fstream>

/*
bool Logging::isInit = false;
bool Logging::enable = true;
Logging Logging::stream;
*/

//std::vector<LoggingHandler*> Logging::handlers;

const char* Logging::endl = "\n";

Logging Log;

void Logging::init() {
	if (isInit) {
		Log.warning("Logging has already been intialized. Please make sure that the logging module has only called init() once.");
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
	handlers.push_back(handler);
}

void Logging::logFormatted(std::string message, Logging::LogLevel level) {
	for (auto h : handlers) {
		if (h != nullptr)
			h->log(message, level);
	}
}

void Logging::log(std::string message){
	for (auto h : handlers) {
		if (h != nullptr)
			h->logMessage(message);
	}
}

void Logging::info(std::string message) {
	logFormatted(message, Logging::INFO);
}

void Logging::warning(std::string message) {
	logFormatted(message, Logging::WARNING);
}

void Logging::error(std::string message) {
	logFormatted(message, Logging::ERROR);
}

void Logging::debug(std::string message) {
	logFormatted(message, Logging::DEBUG);
}

void Logging::critical(std::string message) {
	logFormatted(message, Logging::CRITICAL);
}

void Logging::exception(std::exception e) {
	//TODO: This does not display e.what()
	logFormatted(e.what(), Logging::EXCEPTION);
}

const char* Logging::logLevelToString(LogLevel level) {
	switch (level) {
		case LogLevel::DEBUG: return "DEBUG: "; break;
		case LogLevel::INFO: return "INFO: "; break;
		case LogLevel::WARNING: return "WARNING: "; break;
		case LogLevel::ERROR: return "ERROR: "; break;
		case LogLevel::CRITICAL: return "CRITICAL: "; break;
		case LogLevel::EXCEPTION: return "EXCEPTION: "; break;
		default: return ""; break;
	}
}

std::string Logging::getBasicLogMessage(const char* message, Logging::LogLevel level) {
	std::stringstream ss;
	ss << Logging::logLevelToString(level);
	ss << message;
	ss << '\n';
	return ss.str();
}

void LoggingHandler::log(std::string message, Logging::LogLevel messageLevel) {
	if (!this->enable) return;

	std::string formattedMessage = Logging::getBasicLogMessage(message.c_str(), messageLevel);

	if (messageLevel >= this->level) {
		this->logMessage(formattedMessage.c_str());
	}
}

Logging& operator << (Logging& logger, const char* val){
	logger.log(val);
	return logger;
}
Logging& operator << (Logging& logger, const char val){
	logger.log(std::to_string(val).c_str());
	return logger;
}
Logging& operator << (Logging& logger, const int val){
	logger.log(std::to_string(val).c_str());
	return logger;
}
Logging& operator << (Logging& logger, const long val){
	logger.log(std::to_string(val).c_str());
	return logger;
}
Logging& operator << (Logging& logger, const float val){
	logger.log(std::to_string(val).c_str());
	return logger;
}
Logging& operator << (Logging& logger, const double val){
	logger.log(std::to_string(val).c_str());
	return logger;
}
Logging& operator << (Logging& logger, const std::string val){
	logger.log(val.c_str());
	return logger;
}
Logging& operator << (Logging& logger, const Logging::LogLevel& level){
	logger.log(Logging::logLevelToString(level));
	return logger;
}

//void LoggingHandler::logMessage(const char* message, Logging::LogLevel messageLevel) {}

void ConsoleLoggingHandler::logMessage(std::string message) {
	std::cout << message;
}

void FileOutputLoggingHandler::logMessage(std::string message) {
	if(!outputStream.is_open()){
		outputStream = std::ofstream(this->filePath);

		if(!outputStream.is_open()){
			throw LoggingException("Could not log to file set");
		}
	}

	outputStream << message;
}

void FileOutputLoggingHandler::setFilePath(std::string filePath) {

	outputStream = std::ofstream(filePath);
	if (outputStream.is_open()) {
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

