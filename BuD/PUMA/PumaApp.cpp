#include "PumaApp.h"

#include <GUI/MainGuiLayer.h>

PumaApp::PumaApp()
	: m_Scene(), m_MainDataLayer(m_Scene)
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
