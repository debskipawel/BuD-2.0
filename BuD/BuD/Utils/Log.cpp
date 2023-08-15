#include "bud_pch.h"
#include "Log.h"

#include "Clock.h"

namespace BuD
{
	const std::queue<Log::LogRecord>& Log::GetAllLogs()
	{
#ifdef _DEBUG
		return s_LogMessagesQueue;
#else
		return {};
#endif
	}

	void Log::Write(std::string message, LogSeverity severity)
	{
#ifdef _DEBUG
		auto timepoint = Clock::Now();

		LogRecord logRecord{ timepoint, severity, message };

		s_LogMessagesQueue.push(logRecord);

		for (auto& handle : s_LogMessagesHandles)
		{
			handle(logRecord);
		}
#endif
	}
	
	void Log::RegisterLogHandle(std::function<void(const LogRecord&)> handle)
	{
#ifdef _DEBUG
		s_LogMessagesHandles.push_back(handle);
#endif
	}
}
