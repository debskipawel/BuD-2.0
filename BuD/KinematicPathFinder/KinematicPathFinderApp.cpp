#include "KinematicPathFinderApp.h"

KinematicPathFinderApp::KinematicPathFinderApp()
	: m_MainDataLayer(), m_MainGuiLayer(m_MainDataLayer), m_MouseBehaviorLayer(m_MainDataLayer)
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

auto KinematicPathFinderApp::OnConcreteEvent(BuD::MouseButtonDownEvent& e) -> void
{
	switch (e.m_Button)
	{
		case BuD::MouseCode::LEFT:
		{
			m_MouseBehaviorLayer.OnLeftButtonDown(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::MIDDLE:
		{
			m_MouseBehaviorLayer.OnMiddleButtonDown(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::RIGHT:
		{
			m_MouseBehaviorLayer.OnRightButtonDown(e.m_PosX, e.m_PosY);
			break;
		}
		default:
		{
			break;
		}
	}
}

auto KinematicPathFinderApp::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) -> void
{
	switch (e.m_Button)
	{
		case BuD::MouseCode::LEFT:
		{
			m_MouseBehaviorLayer.OnLeftButtonUp(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::MIDDLE:
		{
			m_MouseBehaviorLayer.OnMiddleButtonUp(e.m_PosX, e.m_PosY);
			break;
		}
		case BuD::MouseCode::RIGHT:
		{
			m_MouseBehaviorLayer.OnRightButtonUp(e.m_PosX, e.m_PosY);
			break;
		}
		default:
		{
			break;
		}
	}
}

auto KinematicPathFinderApp::OnConcreteEvent(BuD::MouseMovedEvent& e) -> void
{
	m_MouseBehaviorLayer.OnMouseMove(e.m_PosX, e.m_PosY);
}
