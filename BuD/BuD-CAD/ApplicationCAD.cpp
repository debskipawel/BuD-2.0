#include "ApplicationCAD.h"

#include <filesystem>
#include <string>

ApplicationCAD::ApplicationCAD()
{
	m_GuiLayer = std::make_unique<MainGuiLayer>(m_ViewModel);

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

	auto torusCount = 1000;
	for (int i = 0; i < torusCount; i++)
	{
		auto position = dxm::Vector3::Zero + 2 * (i - torusCount / 2) * dxm::Vector3::UnitZ;
		m_ViewModel.m_ObjectListViewModel.m_Scene.CreateTorus(position);
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

	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

	BuD::Renderer::Render(scene.m_Scene);

	viewportVM.m_ViewportImage = BuD::Renderer::EndTarget();
}

void ApplicationCAD::OnGuiRender()
{
	m_GuiLayer->DrawGui();
}

void ApplicationCAD::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
	if (m_MoveMouse)
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene.m_Scene;
		auto camera = scene.ActiveCamera();

		camera->RotateCamera(0.005 * e.m_OffsetX, 0.005 * e.m_OffsetY);
	}
}

void ApplicationCAD::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
{
	if (e.m_Button == BuD::MouseCode::MIDDLE)
	{
		m_MoveMouse = true;
	}

	auto& appState = m_ViewModel.m_AppStateViewModel;

	if (e.m_Button == BuD::MouseCode::LEFT && appState.m_AppState != AppState::IDLE)
	{
		m_InAction = true;
	}
}

void ApplicationCAD::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
{
	if (e.m_Button == BuD::MouseCode::MIDDLE)
	{
		m_MoveMouse = false;
	}

	if (e.m_Button == BuD::MouseCode::LEFT)
	{
		m_InAction = false;
	}
}

void ApplicationCAD::OnConcreteEvent(BuD::MouseScrolledEvent& e)
{
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene.m_Scene;
	auto camera = scene.ActiveCamera();

	camera->Zoom(-0.03f * e.m_WheelDelta);
}

void ApplicationCAD::OnConcreteEvent(BuD::KeyDownEvent& e)
{
	if (!m_InAction)
	{
		std::map<BuD::KeyboardKeys, AppState> stateMap =
		{
			{ BuD::KeyboardKeys::D1, AppState::IDLE },
			{ BuD::KeyboardKeys::D2, AppState::MOVE },
			{ BuD::KeyboardKeys::D3, AppState::ROTATE },
			{ BuD::KeyboardKeys::D4, AppState::SCALE },
		};

		for (auto& [key, state] : stateMap)
		{
			if (key == e.m_Key)
			{
				m_ViewModel.m_AppStateViewModel.m_AppState = state;
				break;
			}
		}
	}
}

void ApplicationCAD::OnConcreteEvent(BuD::KeyReleaseEvent& e)
{
}
