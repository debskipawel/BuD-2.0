#include "ApplicationCNC.h"

#include <Applications/CNC/GUI/MainGuiLayerCNC.h>

ApplicationCNC::ApplicationCNC()
	: m_MainDataLayer()
{
	m_GuiLayer = std::make_unique<MainGuiLayerCNC>(m_MainDataLayer);
}

void ApplicationCNC::OnUpdate(float deltaTime)
{
}

void ApplicationCNC::OnRender()
{
}

void ApplicationCNC::OnGuiRender()
{
	m_GuiLayer->DrawGui();
}

void ApplicationCNC::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
}

void ApplicationCNC::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
{
}

void ApplicationCNC::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
{
}

void ApplicationCNC::OnConcreteEvent(BuD::MouseScrolledEvent& e)
{
}

void ApplicationCNC::OnConcreteEvent(BuD::KeyDownEvent& e)
{
}

void ApplicationCNC::OnConcreteEvent(BuD::KeyReleaseEvent& e)
{
}
