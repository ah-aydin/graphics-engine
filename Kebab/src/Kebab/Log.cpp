#include "kbbpch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#ifdef GRAPHICS_API_OPENGL
#define CORE_LOG_FILE_NAME "gl.core.log"
#define CLIENT_LOG_FILE_NAME "gl.client.log"
#define LOG_FILE_START "OpenGl log file"
#elif GRAPHICS_API_VULKAN
#define CORE_LOG_FILE_NAME "vk.core.log"
#define CLIENT_LOG_FILE_NAME "vk.client.log"
#define LOG_FILE_START "Vulkan log file"
#else
error (No graphics api was specified)
#endif

namespace kbb
{
#ifdef _DEBUG
	spdlog::logger Log::s_CoreLogger("DUMMY");
	spdlog::logger Log::s_ClientLogger("DUMMY");
#endif

	void Log::Init()
	{
#ifdef _DEBUG
		time_t now = time(NULL);
		char date[26];
		ctime_s(date, sizeof(date), &now);

		std::string pattern = "%^[%T] %n: %v%$";

		{
			auto core_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			core_console_sink->set_level(spdlog::level::trace);
			core_console_sink->set_pattern(pattern);

			auto core_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(CORE_LOG_FILE_NAME, true);
			core_file_sink->set_pattern(pattern);
			core_file_sink->set_level(spdlog::level::trace);

			s_CoreLogger = spdlog::logger("CORE", { core_console_sink, core_file_sink });

			s_CoreLogger.info(LOG_FILE_START);
			s_CoreLogger.info(date);
		}
		{
			auto client_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			client_console_sink->set_level(spdlog::level::trace);
			client_console_sink->set_pattern(pattern);

			auto client_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(CLIENT_LOG_FILE_NAME, true);
			client_file_sink->set_pattern(pattern);
			client_file_sink->set_level(spdlog::level::trace);
			
			s_ClientLogger = spdlog::logger("CLIENT", { client_console_sink, client_file_sink });

			s_ClientLogger.info(LOG_FILE_START);
			s_ClientLogger.info(date);
		}
#endif
	}
}
