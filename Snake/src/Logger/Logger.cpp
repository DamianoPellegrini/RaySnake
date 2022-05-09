#include "pch.h"
#include "Logger.h"

#include <iostream>
#include <sstream>
#include "date.h"	//Howard Hinnant's library
#include <chrono>

Logger* Logger::s_Instance = nullptr;

Logger::Logger() {
	Logger::s_Instance = this;
	std::cout.sync_with_stdio(false);
}

void Logger::SetLevelColor(Level lv) const {
	switch (lv) {
	case Level::NONE:
		std::cout << "\033[m"/*reset*/;
		break;
	case Level::TRACE:
		std::cout << "\033[2m"/*dark*/ << "\033[37m"/*white*/;
		break;
	case Level::INFO:
		std::cout << "\033[32m"/*green*/;
		break;
	case Level::WARNING:
		std::cout << "\u001b[33;1m"/*bright yellow*/;
		break;
	case Level::ERROR:
		std::cout << "\u001b[31;1m"/*bright red*/;
		break;
	case Level::FATAL:
		std::cout << "\u001b[37;1m"/*bright white*/ << "\033[2m"/*dark*/ << "\033[41m"/*on_red*/;
		break;
	}
}

std::string Logger::GetLevelName(Level lv) const {
	std::string name = "";
	switch (lv) {
	case Level::NONE:
		name = "";
		break;
	case Level::TRACE:
		name = "TRACE";
		break;
	case Level::INFO:
		name = "INFO";
		break;
	case Level::WARNING:
		name = "WARNING";
		break;
	case Level::ERROR:
		name = "ERROR";
		break;
	case Level::FATAL:
		name = "FATAL";
		break;
	}

	return name;
}

Logger* Logger::Get() {
	if (s_Instance == nullptr)
		return new Logger();

	return s_Instance;
}

void Logger::Log(std::string& log, Level lv) const {
#ifdef NDEBUG
	return;
#endif
	using namespace date;	//Provide the operator<< for the time_point class
	std::stringstream ss;
	ss << "[" << floor<std::chrono::seconds>(std::chrono::system_clock::now()) << "][" << GetLevelName(lv) << "]:\t" << log;
	SetLevelColor(lv);
	std::cout << ss.str() << std::endl;
	SetLevelColor(Level::NONE);
}

void Logger::Log(const char* log, Level lv) const {
	Log(std::string(log), lv);
}

void Logger::Log(const ILoggable& log, Level lv) const {
	Log(log.ToString(), lv);
}