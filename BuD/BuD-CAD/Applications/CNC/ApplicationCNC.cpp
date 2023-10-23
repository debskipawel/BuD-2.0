#include "ApplicationCNC.h"

#include <Applications/CNC/GUI/MainGuiLayerCNC.h>
#include <Applications/CNC/Behaviors/Mouse/MainMouseBehaviorLayerCNC.h>

ApplicationCNC::ApplicationCNC()
	: m_MainDataLayer()
{
	m_GuiLayer = std::make_unique<MainGuiLayerCNC>(m_MainDataLayer);

	m_MouseBehaviorLayer = std::make_unique<MainMouseBehaviorLayerCNC>(m_MainDataLayer);

	m_Grid = std::make_unique<Grid>(m_MainDataLayer.m_SceneDataLayer.m_Scene);
}

void ApplicationCNC::OnUpdate(float deltaTime)
{
}

void ApplicationCNC::OnRender()
{
	auto& viewportVM = m_MainDataLayer.m_ViewportDataLayer;

	BuD::Renderer::BeginTarget(viewportVM.m_ViewportWidth, viewportVM.m_ViewportHeight);

	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_Scene;

	BuD::Renderer::Render(scene);

	viewportVM.m_ViewportImage = BuD::Renderer::EndTarget();
}

void ApplicationCNC::OnGuiRender()
{
	m_GuiLayer->DrawGui();
}

void ApplicationCNC::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
	m_MouseBehaviorLayer->OnMouseMove(e.m_OffsetX, e.m_OffsetY);
}

void ApplicationCNC::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
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

void ApplicationCNC::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
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

void ApplicationCNC::OnConcreteEvent(BuD::MouseScrolledEvent& e)
{
	m_MouseBehaviorLayer->OnScroll(e.m_PosX, e.m_PosY, e.m_WheelDelta);
}

void ApplicationCNC::OnConcreteEvent(BuD::KeyDownEvent& e)
{
}

void ApplicationCNC::OnConcreteEvent(BuD::KeyReleaseEvent& e)
{
}
