#include "PumaApp.h"

#include <GUI/MainGuiLayer.h>

PumaApp::PumaApp()
	: m_Scene(), m_MainDataLayer(m_Scene), m_Mesh(m_Scene)
{
	m_MainGuiLayer = std::make_unique<MainGuiLayer>(m_MainDataLayer);
}

auto PumaApp::OnUpdate(float deltaTime) -> void
{
	return;
}

auto PumaApp::OnRender() -> void
{
	auto& viewportVM = m_MainDataLayer.m_ViewportDataLayer;

	BuD::Renderer::BeginTarget(viewportVM.m_ViewportWidth, viewportVM.m_ViewportHeight);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
	BuD::Renderer::Render(m_Scene);

	viewportVM.m_ViewportImage = BuD::Renderer::EndTarget();
}

auto PumaApp::OnGuiRender() -> void
{
	m_MainGuiLayer->DrawGui();
}

auto PumaApp::OnConcreteEvent(BuD::KeyDownEvent& e) -> void
{
	if (e.m_Key == BuD::KeyboardKeys::Q)
	{
		m_MouseMove = true;
	}
}

auto PumaApp::OnConcreteEvent(BuD::KeyReleaseEvent& e) -> void
{
	if (e.m_Key == BuD::KeyboardKeys::Q)
	{
		m_MouseMove = false;
	}
}

auto PumaApp::OnConcreteEvent(BuD::MouseScrolledEvent& e) -> void
{
	auto camera = m_Scene.ActiveCamera();

	camera->Zoom(-0.01f * e.m_WheelDelta);
}

auto PumaApp::OnConcreteEvent(BuD::MouseMovedEvent& e) -> void
{
	if (m_MouseMove)
	{
		auto camera = m_Scene.ActiveCamera();

		camera->RotateCamera(-0.03f * e.m_OffsetX, 0.03 * e.m_OffsetY);
	}
}
