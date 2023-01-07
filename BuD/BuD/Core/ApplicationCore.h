#pragma once

#include "LayerStack.h"

#include <Window/Window.h>

namespace BuD
{
	extern std::shared_ptr<AppLayer> CreateClientApp();

	class ApplicationCore : public IEventDispatchable
	{
	public:
		void Run();

		void PushLayer(std::shared_ptr<AppLayer> layer);

		void OnEvent(Event& e) override;
		void OnConcreteEvent(WindowClosedEvent& e) override;

	private:
		ApplicationCore();
		ApplicationCore(const ApplicationCore& other) = delete;
		ApplicationCore operator=(const ApplicationCore& other) = delete;

		std::unique_ptr<Window> m_Window;

		float m_LastFrameTime;

		bool m_Minimized;
		bool m_ShouldRun;

		LayerStack m_LayerStack;

		inline static std::shared_ptr<ApplicationCore> s_App = nullptr;

	public:
		inline static std::shared_ptr<ApplicationCore> Get()
		{
			if (!s_App)
			{
				s_App = std::shared_ptr<ApplicationCore>(new ApplicationCore());
			}

			return s_App;
		}

		inline static void Close()
		{
			if (!s_App)
			{
				return;
			}

			s_App.reset();
		}
	};
}
