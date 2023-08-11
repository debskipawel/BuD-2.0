#include "ApplicationCAD.h"

#include <filesystem>
#include <string>

#include <Behaviors/Mouse/MainMouseBehaviorLayer.h>
#include <Behaviors/Keyboard/MainKeyboardBehaviorLayer.h>

#include <GUI/MainGuiLayer.h>

ApplicationCAD::ApplicationCAD()
	: m_Scene(), m_ViewModel(m_Scene)
{
	m_GuiLayer = std::make_unique<MainGuiLayer>(m_ViewModel);

	m_MouseBehaviorLayer = std::make_unique<MainMouseBehaviorLayer>(m_ViewModel);
	m_KeyboardBehaviorLayer = std::make_unique<MainKeyboardBehaviorLayer>(m_ViewModel);

	auto currentPath = std::filesystem::current_path();

	BuD::Log::RegisterLogHandle(
		[](const BuD::Log::LogRecord& record)
		{
			if (record.severity != BuD::LogSeverity::LS_ERROR)
			{
				return;
			}

			auto message = std::format(L"[{}] {}\n", record.time.Format(), record.message);

			OutputDebugStringW(message.c_str());
		}
	);

	// TODO: delete this code. just temporary startup benchmarking stuff.
	constexpr auto torusCount = 1000;
	for (int i = 0; i < torusCount; i++)
	{
		auto position = dxm::Vector3::Zero + 2 * (i - torusCount / 2) * dxm::Vector3::UnitZ;
		m_Scene.CreatePoint(position);
	}
}

void ApplicationCAD::OnUpdate(float deltaTime)
{
}

void ApplicationCAD::OnRender()
{
	auto& viewportVM = m_ViewModel.m_ViewportViewModel;

	BuD::Renderer::BeginTarget(viewportVM.m_ViewportWidth, viewportVM.m_ViewportHeight);
	
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

	auto& scene = m_Scene;

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
