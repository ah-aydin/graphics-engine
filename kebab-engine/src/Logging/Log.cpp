#include "Log.h"

#include <fstream>
#include <ctime>
#include <cstdio>
#include <cstdarg>

#include <Windows.h>

namespace kbb
{
#ifndef NDEBUG
	void log_reset()
	{
		// Create a new log file
		std::ofstream out(GL_LOG_FILE);

		// Put the current time in the file
		time_t now = time(NULL);
		char date[26];
		ctime_s(date, sizeof(date), &now);
		out << FILE_START;
		out << "Date: " << date << "\n";
	}

	void log_info(const char* message, ...)
	{
		// Open the log file in append mode
		std::ofstream out(GL_LOG_FILE, std::ios::app);
		if (out.is_open() == false)
		{
			return;
		}

		// Print the log to file
		va_list args;

		va_start(args, message);
		char buffer[4096];
		vsprintf_s(buffer, message, args);
		fprintf(stdout, "%s\n", buffer);
		out << buffer << "\n";
		va_end(args);
	}

	void log_error(const char* message, ...)
	{
		// Open the log file in append mode
		std::ofstream out(GL_LOG_FILE, std::ios::app);
		if (out.is_open() == false)
		{
			return;
		}

		// Print the log to file and stderr
		va_list args;

		va_start(args, message);
		char buffer[4096];
		vsprintf_s(buffer, message, args);
		out << buffer << "\n";

		HANDLE hConsoleErr = GetStdHandle(STD_ERROR_HANDLE);
		SetConsoleTextAttribute(hConsoleErr, FOREGROUND_RED);

		fprintf(stderr, "%s", buffer);
		fprintf(stderr, "\n");

		SetConsoleTextAttribute(hConsoleErr, FOREGROUND_INTENSITY);

		va_end(args);
	}

	void log_error_exception(const char* message, ...)
	{
		// Open the log file in append mode
		std::ofstream out(GL_LOG_FILE, std::ios::app);
		if (out.is_open() == false)
		{
			return;
		}

		// Print the log to file and stderr
		va_list args;

		va_start(args, message);
		char buffer[4096];
		vsprintf_s(buffer, message, args);
		out << buffer << "\n";

		HANDLE hConsoleErr = GetStdHandle(STD_ERROR_HANDLE);
		SetConsoleTextAttribute(hConsoleErr, FOREGROUND_RED);

		fprintf(stderr, "%s", buffer);
		fprintf(stderr, "\n");

		SetConsoleTextAttribute(hConsoleErr, FOREGROUND_INTENSITY);

		va_end(args);

		throw std::runtime_error(buffer);
	}
#else
	void log_reset() { }
	void log_info(const char* message, ...) { }
	void log_error(const char* message, ...) { }
	void log_error_exception(const char* message, ...) { }
#endif
}