#pragma once
#include <ArduinoSTL.h>
#include <M2M_Logger.h>
#include <SD.h>

#define LOGFILE		"log.txt"


class MonitoringLogger { 
public:
	MonitoringLogger() {  }

	static MonitoringLogger* const Instance() {
		if(instance == nullptr) {
			instance = new MonitoringLogger;
		}
		return instance;
	}

	static void Start(Print* serialLog, LogLevel logLevel) {
		auto instance = MonitoringLogger::Instance();
		instance->log.begin(serialLog, logLevel);
	}

	static void SetUpFileLogger() {
		auto instance = MonitoringLogger::Instance();
		instance->logFile = SD.open(LOGFILE, FILE_WRITE);
		if(instance->logFile) {
			instance->log.setFileLogger(&instance->logFile);
			instance->log.info(F("Log file initialized"));
			instance->fileInitialized = true;
		} else {
			instance->fileInitialized = false;
		}
	}

	static void ChangeLogLevel(LogLevel logLevel) {
		auto instance = MonitoringLogger::Instance();
		instance->log.setLogLevel(logLevel);
	}

	template<class T,typename... Args>
	static void LogInfo(T message, Args... args) {
		auto instance = MonitoringLogger::Instance();
		instance->log.info(message, args...);
	}

	template<class T, typename... Args>
	static void LogError(T message, Args... args) {
		auto instance = MonitoringLogger::Instance();
		instance->log.error(message, args...);
	}

	template<class T, typename... Args>
	static void LogDebug(T message, Args... args) {
		auto instance = MonitoringLogger::Instance();
		instance->log.debug(message, args...);
	}

private:
	Logger log;
	File logFile;
	bool fileInitialized = false;
	static MonitoringLogger* instance;
};

