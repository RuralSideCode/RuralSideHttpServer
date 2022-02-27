#pragma once

#include <string>
#include <vector>
#include <exception>
#include <fstream>

#define LOG_DEBUG 0x01
#define LOG_INFO 0x02
#define LOG_WARNING 0x04
#define LOG_ERROR 0x08
#define LOG_CRITICAL 0x10
#define LOG_EXCEPTION 0x20

class LoggingHandler;

class Logging {

public:
	enum LogLevel {
		DEBUG = LOG_DEBUG,
		INFO = LOG_INFO,
		WARNING = LOG_WARNING,
		ERROR = LOG_ERROR,
		CRITICAL = LOG_CRITICAL,
		EXCEPTION = LOG_EXCEPTION
	};

	/**
	 * Initializes the logger
	 */
	static void init();

	/**
	 * Closes the logger
	 */
	static void close();

	/**
	 * When enable is true, the logger will log to all the log handlers
	 */
	static bool enable;

	/**
	 * Adds a logging handler to the logger
	 * @param handler The handler to add
	 */
	static void addHandler(LoggingHandler* handler);
	
	//TODO: Add remove handler
	//void removeHandler(LoggingHandler& handler);

	//Logging functions
	static void log(const char* message, LogLevel level);
	static void info(const char* message);
	static void debug(const char* message);
	static void warning(const char* message);
	static void error(const char* message);
	static void critical(const char* message);
	static void exception(std::exception e);

	static const char* logLevelToString(LogLevel level);
	static std::string getBasicLogMessage(const char* message, Logging::LogLevel level);

private:
	static std::vector<LoggingHandler*> handlers;

	static bool isInit;
};

class LoggingException : public std::exception {
public:
	LoggingException(const char* message);

	virtual const char* what();

private:
	const char* message;
};

class LoggingHandler {
public:
	Logging::LogLevel level = Logging::LogLevel::DEBUG;

	void log(const char* message, Logging::LogLevel messageLevel);

	virtual void logMessage(const char* message, Logging::LogLevel messageLevel) = 0;

	bool enable = true;
};

class ConsoleLoggingHandler : public LoggingHandler {
public:
	virtual void logMessage(const char* message, Logging::LogLevel messageLevel);
};

class FileOutputLoggingHandler : public LoggingHandler {
public:
	virtual void logMessage(const char* message, Logging::LogLevel mesageLevel);

	void setFilePath(std::string filePath);

private:
	std::string filePath;
};
