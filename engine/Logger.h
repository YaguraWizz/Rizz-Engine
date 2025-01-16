#pragma once
#include <macro.h>
#include <spdlog/spdlog.h>


namespace RIZZ {

	class Logger {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClinetLogger() { return s_ClinetLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClinetLogger;
	};

}

#define RZ_CORE_TRACE(...)		::RIZZ::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define RZ_CORE_INFO(...)		::RIZZ::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define RZ_CORE_WARN(...)		::RIZZ::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define RZ_CORE_ERROR(...)		::RIZZ::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define RZ_CORE_DEBUG(...)		::RIZZ::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define RZ_CORE_CRITICAL(...)	::RIZZ::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define RZ_TRACE(...)			::RIZZ::Logger::GetClinetLogger()->trace(__VA_ARGS__)
#define RZ_INFO(...)			::RIZZ::Logger::GetClinetLogger()->info(__VA_ARGS__)
#define RZ_WARN(...)			::RIZZ::Logger::GetClinetLogger()->warn(__VA_ARGS__)
#define RZ_ERROR(...)			::RIZZ::Logger::GetClinetLogger()->error(__VA_ARGS__)
#define RZ_DEBUG(...)			::RIZZ::Logger::GetClinetLogger()->debug(__VA_ARGS__)
#define RZ_CRITICAL(...)		::RIZZ::Logger::GetClinetLogger()->critical(__VA_ARGS__)

