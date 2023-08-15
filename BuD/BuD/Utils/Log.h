#pragma once

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "Timepoint.h"

namespace BuD
{
	enum LogSeverity
	{
		LS_INFO = 0,
		LS_WARNING = 1,
		LS_ERROR = 2
	};

	class Log
	{
	public:
		struct LogRecord
		{
			Timepoint time;
			LogSeverity severity;
			std::string message;
		};

		static const std::queue<LogRecord>& GetAllLogs();

		static void Write(std::string message, LogSeverity severity);
		inline static void WriteInfo(std::string message) { Write(message, LS_INFO); }
		inline static void WriteWarning(std::string message) { Write(message, LS_WARNING); }
		inline static void WriteError(std::string message) { Write(message, LS_ERROR); }

		static void RegisterLogHandle(std::function<void(const LogRecord&)> handle);
	
#ifdef _DEBUG
	private:
		inline static std::queue<LogRecord> s_LogMessagesQueue = {};
		inline static std::vector<std::function<void(const LogRecord&)>> s_LogMessagesHandles = {};
#endif
	};
}
