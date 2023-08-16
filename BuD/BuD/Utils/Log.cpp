#include "bud_pch.h"
#include "Log.h"

#include "Clock.h"

namespace BuD
{
	const std::queue<Log::LogRecord>& Log::GetAllLogs()
	{
		return s_LogMessagesQueue;
	}

	void Log::Write(std::string message, LogSeverity severity)
	{
		auto timepoint = Clock::Now();

		LogRecord logRecord{ timepoint, severity, message };

		s_LogMessagesQueue.push(logRecord);

		for (auto& handle : s_LogMessagesHandles)
		{
			handle(logRecord);
		}
	}
	
	void Log::RegisterLogHandle(std::function<void(const LogRecord&)> handle)
	{
		s_LogMessagesHandles.push_back(handle);
	}
}
