#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace kbb
{
	class Log
	{
	public:
		static void Init();

#ifdef _DEBUG
		inline static spdlog::logger& GetCoreLogger() { return s_CoreLogger; }
		inline static spdlog::logger& GetClientLogger() { return s_ClientLogger; }

	private:
		static spdlog::logger s_CoreLogger;

		static spdlog::logger s_ClientLogger;
#endif
	};
}

#ifdef NDEBUG
	#define KBB_CORE_TRACE(...)
	#define KBB_CORE_INFO(...)
	#define KBB_CORE_WARN(...)
	#define KBB_CORE_ERROR(...)

	#define KBB_TRACE(...)
	#define KBB_INFO(...)
	#define KBB_WARN(...)
	#define KBB_ERROR(...)
	
	#define KBB_EXCEPTION(...)
#else
	#define KBB_CORE_TRACE(...) kbb::Log::GetCoreLogger().trace(__VA_ARGS__);
	#define KBB_CORE_INFO(...)	kbb::Log::GetCoreLogger().info(__VA_ARGS__);
	#define KBB_CORE_WARN(...)	kbb::Log::GetCoreLogger().warn(__VA_ARGS__);
	#define KBB_CORE_ERROR(...) kbb::Log::GetCoreLogger().error(__VA_ARGS__);

	#define KBB_TRACE(...)		kbb::Log::GetClientLogger().trace(__VA_ARGS__);
	#define KBB_INFO(...)		kbb::Log::GetClientLogger().info(__VA_ARGS__);
	#define KBB_WARN(...)		kbb::Log::GetClientLogger().warn(__VA_ARGS__);
	#define KBB_ERROR(...)		kbb::Log::GetClientLogger().error(__VA_ARGS__);

	#define KBB_EXCEPTION(...)	kbb::Log::GetCoreLogger().error(__VA_ARGS__); assert(false);
#endif
