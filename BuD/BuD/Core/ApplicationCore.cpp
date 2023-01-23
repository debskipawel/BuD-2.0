#include "bud_pch.h"
#include "ApplicationCore.h"

#include <Utils/Clock.h>

#include <Event/WindowEvents.h>

#include <Renderer/Renderer.h>

namespace BuD
{
	ApplicationCore::ApplicationCore()
		: m_Minimized(), m_LastFrameTime(0.0f), m_ShouldRun()
	{
	}

	void ApplicationCore::Run()
	{
		m_Window = std::make_shared<Win32Window>();

		Renderer::Initialize(m_Window);
		Clock::Initialize();

		m_GuiLayer = std::make_unique<GuiLayer>(m_Window);
		PushLayer(CreateClientApp());

		m_Window->Show();

		m_ShouldRun = true;

		while (m_ShouldRun)
		{
			auto currentFrameTime = Clock::Now();
			auto deltaTime = currentFrameTime - m_LastFrameTime;

			m_LastFrameTime = currentFrameTime;

			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			Renderer::BeginFrame();

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			m_GuiLayer->BeginFrame();

			for (auto& layer : m_LayerStack)
			{
				layer->OnGuiRender();
			}

			m_GuiLayer->EndFrame();

			Renderer::EndFrame();

			m_Window->ProcessEvents();
		}
	}
	
	void ApplicationCore::PushLayer(std::shared_ptr<AppLayer> layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	
	void ApplicationCore::OnEvent(Event& e)
	{
		e.Visit(*this);

		for (auto& layer : m_LayerStack)
		{
			if (e.m_Handled)
			{
				break;
			}

			layer->OnEvent(e);
		}
	}
	
	void ApplicationCore::OnConcreteEvent(WindowClosedEvent& e)
	{
		m_ShouldRun = false;
	}

	void ApplicationCore::OnConcreteEvent(WindowResizedEvent& e)
	{
		m_Minimized = e.m_Minimized;

		if (!m_Minimized)
		{
			Renderer::UpdateBuffersSize(e.m_Width, e.m_Height);
		}
	}
}
