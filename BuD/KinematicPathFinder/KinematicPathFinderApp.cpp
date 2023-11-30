#include "KinematicPathFinderApp.h"

KinematicPathFinderApp::KinematicPathFinderApp()
	: m_MainDataLayer(), m_MainGuiLayer(m_MainDataLayer)
{
}

auto KinematicPathFinderApp::OnUpdate(float deltaTime) -> void
{
	return;
}

auto KinematicPathFinderApp::OnGuiRender() -> void
{
	m_MainGuiLayer.DrawGui();
}

auto KinematicPathFinderApp::OnRender() -> void
{
	auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;
	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	BuD::Renderer::BeginTarget(viewportDataLayer.m_Width, viewportDataLayer.m_Height);
	BuD::Renderer::Clear(0.2f, 0.5f, 0.6f, 1.0f);

	BuD::Renderer::Render(sceneDataLayer.m_Scene);

	viewportDataLayer.m_ViewportImage = BuD::Renderer::EndTarget();
}
