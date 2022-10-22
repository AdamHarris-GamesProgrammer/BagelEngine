#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Bagel {
	class BG_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _sClientLogger;  }

	private:
		static std::shared_ptr<spdlog::logger> _sCoreLogger;
		static std::shared_ptr<spdlog::logger> _sClientLogger;


	};
}

#define BG_CORE_ERROR(...) ::Bagel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BG_CORE_WARNING(...) ::Bagel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BG_CORE_INFO(...) ::Bagel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BG_CORE_TRACE(...) ::Bagel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BG_CORE_FATAL(...) ::Bagel::log::GetCoreLogger()->critical(__VA_ARGS__)

#define BG_ERROR(...) ::Bagel::Log::GetClientLogger()->error(__VA_ARGS__)
#define BG_WARNING(...) ::Bagel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BG_INFO(...) ::Bagel::Log::GetClientLogger()->info(__VA_ARGS__)
#define BG_TRACE(...) ::Bagel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BG_FATAL(...) ::Bagel::log::GetClientLogger()->critical(__VA_ARGS__)

