#pragma once

#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include <iostream>

#define LOG_DEBUG 0x01
#define LOG_INFO 0x02
#define LOG_WARNING 0x04
#define LOG_ERROR 0x08
#define LOG_CRITICAL 0x10
#define LOG_EXCEPTION 0x20

class LoggingHandler;

class Logging : std::ostream{

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
	void init();

	/**
	 * Closes the logger
	 */
	void close();

	/**
	 * When enable is true, the logger will log to all the log handlers
	 */
	bool enable;

	/**
	 * Adds a logging handler to the logger
	 * @param handler The handler to add
	 */
	void addHandler(LoggingHandler* handler);

	//TODO: Add remove handler
	//void removeHandler(LoggingHandler& handler);

	//Logging functions
	void logFormatted(const char* message, LogLevel level);
	void log(const char* message);
	void info(const char* message);
	void debug(const char* message);
	void warning(const char* message);
	void error(const char* message);
	void critical(const char* message);
	void exception(std::exception e);

	static const char* logLevelToString(LogLevel level);
	static std::string getBasicLogMessage(const char* message, Logging::LogLevel level);

	static const char* endl;

	/*
	Logging& operator << (const char* val);
	Logging& operator << (const char val);
	Logging& operator << (const int val);
	Logging& operator << (const long val);
	Logging& operator << (const float val);
	Logging& operator << (const double val);
	Logging& operator << (const std::string val);
	Logging& operator << (const LogLevel& level);
	*/

	friend Logging& operator << (Logging& logger, const char* val);
	friend Logging& operator << (Logging& logger, const char val);
	friend Logging& operator << (Logging& logger, const int val);
	friend Logging& operator << (Logging& logger, const long val);
	friend Logging& operator << (Logging& logger, const float val);
	friend Logging& operator << (Logging& logger, const double val);
	friend Logging& operator << (Logging& logger, const std::string val);
	friend Logging& operator << (Logging& logger, const Logging::LogLevel& level);

private:
	 std::vector<LoggingHandler*> handlers;

	 bool isInit;
};

typedef Logging Logger;

extern Logging Log;

Logging& operator << (Logging& logger, const char* val);
Logging& operator << (Logging& logger, const char val);
Logging& operator << (Logging& logger, const int val);
Logging& operator << (Logging& logger, const long val);
Logging& operator << (Logging& logger, const float val);
Logging& operator << (Logging& logger, const double val);
Logging& operator << (Logging& logger, const std::string val);
Logging& operator << (Logging& logger, const Logging::LogLevel& level);

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

	virtual void logMessage(const char* message) = 0;

	bool enable = true;
};

class ConsoleLoggingHandler : public LoggingHandler {
public:
	virtual void logMessage(const char* message);
};

class FileOutputLoggingHandler : public LoggingHandler {
public:

	//I only put inline to look cool
	inline ~FileOutputLoggingHandler() {
		if(outputStream.is_open())
			outputStream.close();
	}

	virtual void logMessage(const char* message);

	void setFilePath(std::string filePath);

private:
	std::string filePath;
	std::ofstream outputStream;
};
