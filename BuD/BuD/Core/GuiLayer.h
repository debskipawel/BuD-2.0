#pragma once

#include <memory>

#include <Window/Win32Window.h>

namespace BuD
{
	class GuiLayer
	{
	public:
		GuiLayer(std::shared_ptr<Win32Window> window);
		~GuiLayer();

		void BeginFrame();
		void EndFrame();
	};
}
