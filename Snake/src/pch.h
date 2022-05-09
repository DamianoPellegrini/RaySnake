#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include <memory>
#include <functional>

#include <raylib.h>

#include "Logger/Logger.h"

#define SLOG_TRACE(x)	::Logger::Get()->Log(x, Logger::Level::TRACE)
#define SLOG_INFO(x)	::Logger::Get()->Log(x, Logger::Level::INFO)
#define SLOG_WARNING(x)	::Logger::Get()->Log(x, Logger::Level::WARNING)
#define SLOG_ERROR(x)	::Logger::Get()->Log(x, Logger::Level::ERROR)
#define SLOG_FATAL(x)	::Logger::Get()->Log(x, Logger::Level::FATAL)

#define BACKGROUND_COLOR {51,51,64,255}

#ifdef DEBUG
#define PROFILING
#endif
