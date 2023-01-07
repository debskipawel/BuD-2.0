#include "bud_pch.h"
#include "Log.h"

#include "Clock.h"

namespace BuD
{
	const std::queue<std::pair<Timepoint, std::wstring>>& BuD::Log::GetAllLogs()
	{
#ifdef _DEBUG
		return s_LogMessagesQueue;
#else
		return {};
#endif
	}

	void Log::Write(std::wstring message)
	{
#ifdef _DEBUG
		auto timepoint = Clock::Now();

		auto formattedMessage = timepoint.Format() + L" " + message;

		s_LogMessagesQueue.push(std::make_pair(timepoint, formattedMessage));

		for (auto& handle : s_LogMessagesHandles)
		{
			handle(formattedMessage);
		}
#endif
	}
	
	void Log::RegisterLogHandle(std::function<void(std::wstring)> handle)
	{
#ifdef _DEBUG
		s_LogMessagesHandles.push_back(handle);
#endif
	}
}
