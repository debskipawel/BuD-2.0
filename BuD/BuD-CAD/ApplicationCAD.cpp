#include "ApplicationCAD.h"

#include <filesystem>
#include <string>

#include <Behaviors/Mouse/MainMouseBehaviorLayer.h>
#include <Behaviors/Keyboard/MainKeyboardBehaviorLayer.h>

#include <GUI/MainGuiLayer.h>

ApplicationCAD::ApplicationCAD()
	: m_MainDataLayer()
{
	m_GuiLayer = std::make_unique<MainGuiLayer>(m_MainDataLayer);

	m_MouseBehaviorLayer = std::make_unique<MainMouseBehaviorLayer>(m_MainDataLayer);
	m_KeyboardBehaviorLayer = std::make_unique<MainKeyboardBehaviorLayer>(m_MainDataLayer);
}

void ApplicationCAD::OnUpdate(float deltaTime)
{
}

void ApplicationCAD::OnRender()
{
	auto& viewportVM = m_MainDataLayer.m_ViewportDataLayer;

	BuD::Renderer::BeginTarget(viewportVM.m_ViewportWidth, viewportVM.m_ViewportHeight);
	
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

	BuD::Renderer::Render(scene.m_Scene);

	viewportVM.m_ViewportImage = BuD::Renderer::EndTarget();
}

void ApplicationCAD::OnGuiRender()
{
	m_GuiLayer->DrawGui();
}

void ApplicationCAD::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
	m_MouseBehaviorLayer->OnMouseMove(e.m_OffsetX, e.m_OffsetY);
}

void ApplicationCAD::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
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

void ApplicationCAD::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
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

void ApplicationCAD::OnConcreteEvent(BuD::MouseScrolledEvent& e)
{
	m_MouseBehaviorLayer->OnScroll(e.m_PosX, e.m_PosY, e.m_WheelDelta);
}

void ApplicationCAD::OnConcreteEvent(BuD::KeyDownEvent& e)
{
	m_KeyboardBehaviorLayer->OnKeyPress(e.m_Key);
}

void ApplicationCAD::OnConcreteEvent(BuD::KeyReleaseEvent& e)
{
	m_KeyboardBehaviorLayer->OnKeyRelease(e.m_Key);
}
