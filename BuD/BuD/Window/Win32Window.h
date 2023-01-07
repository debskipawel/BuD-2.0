#pragma once

#ifdef _WIN32

#include "Window.h"

#include <Windows.h>

namespace BuD
{
	class Win32Window : public Window
	{
	public: 
		Win32Window();
		virtual ~Win32Window() = default;

		virtual void Show() override;
		virtual void ProcessEvents() override;

		virtual int Width() override;
		virtual int Height() override;

		virtual void ToggleFullscreen() override;

		inline HWND Handle() { return m_hwnd; }

	private:
		HWND m_hwnd;
	};
}

#endif
