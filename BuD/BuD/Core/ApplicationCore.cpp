#include "bud_pch.h"
#include "ApplicationCore.h"

#include <Utils/Clock.h>
#include <Profiler/Profiler.h>

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
			Profiler::BeginFrame();

			auto currentFrameTime = Clock::Now();
			auto deltaTime = currentFrameTime - m_LastFrameTime;

			m_LastFrameTime = currentFrameTime;

			Profiler::BeginScope("OnUpdate");

			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			Profiler::EndScope();

			Profiler::BeginScope("Rendering");
			Profiler::BeginScope("Layer rendering");

			Renderer::BeginFrame();

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			Profiler::EndScope();
			Profiler::BeginScope("GUI rendering");

			m_GuiLayer->BeginFrame();

			for (auto& layer : m_LayerStack)
			{
				layer->OnGuiRender();
			}

			m_GuiLayer->EndFrame();

			Profiler::EndScope();

			Profiler::BeginScope("Presenting");
			Renderer::EndFrame();
			Profiler::EndScope();

			Profiler::EndScope();

			Profiler::BeginScope("Events processing");
			m_Window->ProcessEvents();
			Profiler::EndScope();

			Profiler::EndFrame();
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
			Renderer::WindowResized(e.m_Width, e.m_Height);
		}
	}
}
