#include "PumaApp.h"

#include <Behaviors/Keyboard/MainKeyboardBehaviorLayer.h>
#include <Behaviors/Mouse/MainMouseBehaviorLayer.h>

#include <GUI/MainGuiLayer.h>

PumaApp::PumaApp()
	: m_MainDataLayer()
{
	m_KeyboardBehaviorLayer = std::make_unique<MainKeyboardBehaviorLayer>(m_MainDataLayer);
	m_MouseBehaviorLayer = std::make_unique<MainMouseBehaviorLayer>(m_MainDataLayer);

	m_MainGuiLayer = std::make_unique<MainGuiLayer>(m_MainDataLayer);
}

auto PumaApp::OnUpdate(float deltaTime) -> void
{
	return;
}

auto PumaApp::OnRender() -> void
{
	auto& viewport1 = m_MainDataLayer.m_ViewportDataLayer1;

	BuD::Renderer::BeginTarget(viewport1.m_ViewportWidth, viewport1.m_ViewportHeight);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
	//BuD::Renderer::Render(m_Scene);

	viewport1.m_ViewportImage = BuD::Renderer::EndTarget();

	auto& viewport2 = m_MainDataLayer.m_ViewportDataLayer1;

	BuD::Renderer::BeginTarget(viewport2.m_ViewportWidth, viewport2.m_ViewportHeight);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
	//BuD::Renderer::Render(m_Scene);

	viewport2.m_ViewportImage = BuD::Renderer::EndTarget();
}

auto PumaApp::OnGuiRender() -> void
{
	m_MainGuiLayer->DrawGui();
}

auto PumaApp::OnConcreteEvent(BuD::KeyDownEvent& e) -> void
{
	m_KeyboardBehaviorLayer->OnKeyPress(e.m_Key);
}

auto PumaApp::OnConcreteEvent(BuD::KeyReleaseEvent& e) -> void
{
	m_KeyboardBehaviorLayer->OnKeyRelease(e.m_Key);
}

auto PumaApp::OnConcreteEvent(BuD::MouseButtonDownEvent& e) -> void
{
	switch (e.m_Button)
	{
		case BuD::MouseCode::LEFT:
		{
			m_MouseBehaviorLayer->OnLeftButtonDown(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::MIDDLE:
		{
			m_MouseBehaviorLayer->OnMiddleButtonDown(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::RIGHT:
		{
			m_MouseBehaviorLayer->OnRightButtonDown(e.m_PosX, e.m_PosY);
			break;
		}
		default:
		{
			break;
		}
	}
}

auto PumaApp::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) -> void
{
	switch (e.m_Button)
	{
		case BuD::MouseCode::LEFT:
		{
			m_MouseBehaviorLayer->OnLeftButtonUp(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::MIDDLE:
		{
			m_MouseBehaviorLayer->OnMiddleButtonUp(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::RIGHT:
		{
			m_MouseBehaviorLayer->OnRightButtonUp(e.m_PosX, e.m_PosY);
			break;
		}
		default:
		{
			break;
		}
	}
}

auto PumaApp::OnConcreteEvent(BuD::MouseScrolledEvent& e) -> void
{
	m_MouseBehaviorLayer->OnScroll(e.m_PosX, e.m_PosY, e.m_WheelDelta);
}

auto PumaApp::OnConcreteEvent(BuD::MouseMovedEvent& e) -> void
{
	m_MouseBehaviorLayer->OnMove(e.m_PosX, e.m_PosY, e.m_OffsetX, e.m_OffsetY);
}
