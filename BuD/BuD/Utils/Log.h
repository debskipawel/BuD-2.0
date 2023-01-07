#pragma once

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "Timepoint.h"

namespace BuD
{
	class Log
	{
	public:
		static const std::queue<std::pair<Timepoint, std::wstring>>& GetAllLogs();
		static void Write(std::wstring message);
		static void RegisterLogHandle(std::function<void(std::wstring)> handle);
	
#ifdef _DEBUG
	private:
		inline static std::queue<std::pair<Timepoint, std::wstring>> s_LogMessagesQueue = {};
		inline static std::vector<std::function<void(std::wstring)>> s_LogMessagesHandles = {};
#endif
	};
}
