#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Bagel {
	std::shared_ptr<spdlog::logger> Log::_sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::_sClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); //Timestamp, name of logger, message

		_sCoreLogger = spdlog::stdout_color_mt("Bagel");
		_sCoreLogger->set_level(spdlog::level::trace);
		_sClientLogger = spdlog::stdout_color_mt("App");
		_sClientLogger->set_level(spdlog::level::trace);
	}
}


