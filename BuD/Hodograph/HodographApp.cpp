#include "HodographApp.h"

#include <GUI/MainGuiLayer.h>

HodographApp::HodographApp()
	: m_MainDataLayer()
{
	m_GuiLayer = std::make_unique<MainGuiLayer>(m_MainDataLayer);
}

auto HodographApp::OnUpdate(float deltaTime) -> void
{
}

auto HodographApp::OnRender() -> void
{
}

auto HodographApp::OnGuiRender() -> void
{
	m_GuiLayer->DrawGui();
}

auto HodographApp::OnConcreteEvent(BuD::KeyDownEvent& e) -> void
{
}

auto HodographApp::OnConcreteEvent(BuD::KeyReleaseEvent& e) -> void
{
}

auto HodographApp::OnConcreteEvent(BuD::MouseButtonDownEvent& e) -> void
{
}

auto HodographApp::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) -> void
{
}

auto HodographApp::OnConcreteEvent(BuD::MouseScrolledEvent& e) -> void
{
}

auto HodographApp::OnConcreteEvent(BuD::MouseMovedEvent& e) -> void
{
}
