#pragma once

#include "LayerStack.h"

namespace BuD
{
	class ApplicationCore
	{
	public:
		void Run();

	private:
		ApplicationCore();
		ApplicationCore(const ApplicationCore& other) = delete;
		ApplicationCore operator=(const ApplicationCore& other) = delete;

		float m_LastFrameTime;

		bool m_Minimized;
		bool m_ShouldRun;

		LayerStack m_LayerStack;

		static std::shared_ptr<ApplicationCore> s_app;

	public:
		inline static std::shared_ptr<ApplicationCore> Get()
		{
			if (!s_app)
			{
				s_app = std::shared_ptr<ApplicationCore>(new ApplicationCore());
			}

			return s_app;
		}

		inline static void Close()
		{
			if (!s_app)
			{
				return;
			}

			s_app.reset();
		}
	};
}
