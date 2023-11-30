#include "KinematicPathFinderApp.h"

KinematicPathFinderApp::KinematicPathFinderApp()
	: m_KinematicPathScene(), m_ViewportGui("Viewport"), m_AppStateGui(), m_RobotParametersGui()
{
}

auto KinematicPathFinderApp::OnUpdate(float deltaTime) -> void
{
	return;
}

auto KinematicPathFinderApp::OnGuiRender() -> void
{
	m_ViewportGui.DrawGui();
	m_RobotParametersGui.DrawGui();
	m_AppStateGui.DrawGui();
}

auto KinematicPathFinderApp::OnRender() -> void
{
	auto viewportInfo = m_ViewportGui.GetViewportInfo();

	BuD::Renderer::BeginTarget(viewportInfo.m_Width, viewportInfo.m_Height);
	BuD::Renderer::Clear(0.2f, 0.5f, 0.6f, 1.0f);

	BuD::Renderer::Render(m_KinematicPathScene.m_Scene);

	auto texture = BuD::Renderer::EndTarget();

	m_ViewportGui.UpdateImage(texture);
}
