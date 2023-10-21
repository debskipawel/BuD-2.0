#include "ApplicationSwitcher.h"

#include <Applications/CAD/ApplicationCAD.h>

#include <Applications/Switcher/GUI/MainGuiLayerSwitcher.h>

ApplicationSwitcher::ApplicationSwitcher()
	: m_MainDataLayer()
{
	m_MainGuiLayer = std::make_unique<MainGuiLayerSwitcher>(m_MainDataLayer);
}

void ApplicationSwitcher::OnUpdate(float deltaTime)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnUpdate(deltaTime);
	}
}

void ApplicationSwitcher::OnRender()
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnRender();
	}
}

void ApplicationSwitcher::OnGuiRender()
{
	m_MainGuiLayer->DrawGui();

	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnGuiRender();
	}
}

void ApplicationSwitcher::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnConcreteEvent(e);
	}
}

void ApplicationSwitcher::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnConcreteEvent(e);
	}
}

void ApplicationSwitcher::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnConcreteEvent(e);
	}
}

void ApplicationSwitcher::OnConcreteEvent(BuD::MouseScrolledEvent& e)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnConcreteEvent(e);
	}
}

void ApplicationSwitcher::OnConcreteEvent(BuD::KeyDownEvent& e)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnConcreteEvent(e);
	}
}

void ApplicationSwitcher::OnConcreteEvent(BuD::KeyReleaseEvent& e)
{
	if (m_MainDataLayer.m_ActiveApp)
	{
		m_MainDataLayer.m_ActiveApp->OnConcreteEvent(e);
	}
}
