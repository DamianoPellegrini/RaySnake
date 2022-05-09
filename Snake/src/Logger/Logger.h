#pragma once
#ifndef LOG_H
#define LOG_H

#include "ILoggable.h"

#include <string>

class Logger {
public:
	enum class Level {
		NONE = -1,
		TRACE = 0,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	static Logger* Get();

	void Log(std::string& log, Level lv) const;
	void Log(const char* log, Level lv) const;
	void Log(const ILoggable& log, Level lv) const;
private:
	static Logger* s_Instance;

	Logger();

	void SetLevelColor(Level lv) const;
	std::string GetLevelName(Level lv) const;
};

#endif // !LOG_H
