#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>


namespace RIZZ {

	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClinetLogger;

	void Logger::Initialized() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("RIZZ");
		s_CoreLogger->set_level(spdlog::level::trace);


		s_ClinetLogger = spdlog::stdout_color_mt("APP");
		s_ClinetLogger->set_level(spdlog::level::trace);

	}
}