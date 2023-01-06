#include "bud_pch.h"
#include "ApplicationCore.h"

#include <Utils/Clock.h>

namespace BuD
{
	ApplicationCore::ApplicationCore()
		: m_Minimized(), m_LastFrameTime(), m_ShouldRun()
	{
	}

	void ApplicationCore::Run()
	{
		m_LayerStack.PushLayer(CreateClientApp());

		Clock::Init();
		m_ShouldRun = true;

		while (m_ShouldRun)
		{
			float currentFrameTime = Clock::Now();
			float deltaTime = currentFrameTime - m_LastFrameTime;

			m_LastFrameTime = currentFrameTime;

			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}
		}
	}
}
