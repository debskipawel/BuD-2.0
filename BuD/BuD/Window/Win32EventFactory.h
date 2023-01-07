#ifdef _WIN32
#pragma once

#include <memory>
#include <map>
#include <Windows.h>

#include <Input/KeyboardKeys.h>
#include <Input/MouseCode.h>

#include <Event/Event.h>

namespace BuD
{
	class Win32EventFactory
	{
	public:
		static std::unique_ptr<Event> Construct(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		Win32EventFactory() = delete;
		Win32EventFactory(Win32EventFactory& other) = delete;
		void operator=(const Win32EventFactory&) = delete;
	};
}

#endif
