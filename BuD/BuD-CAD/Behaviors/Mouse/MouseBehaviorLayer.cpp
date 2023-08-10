#include "MouseBehaviorLayer.h"

#include <Raycasting/RayIntersectionVisitor.h>

MouseBehaviorLayer::MouseBehaviorLayer(MainViewModel& viewModel)
	: m_ViewModel(viewModel), m_MoveMouse(false)
{
	m_RayFactory = std::make_unique<RayFactory>(viewModel.m_ViewportViewModel, viewModel.m_ObjectListViewModel);
}

void MouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::IDLE)
	{
		if (!IsMouseOnViewport(x, y))
		{
			return;
		}

		auto screenSpace = ViewportScreenSpaceCoords(x, y);
		auto ray = m_RayFactory->CreateRay(screenSpace);

		auto visitor = std::make_unique<RayIntersectionVisitor>(ray);

		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

		bool anyHit = false;

		for (auto& [id, object] : scene.m_ObjectList)
		{
			visitor->Visit(*object.get());
			auto results = visitor->GetLatestResults();

			if (results.m_Hit)
			{
				anyHit = true;
				object->m_Selected = true;
				scene.m_SelectedGroup.Add(object);
			}
		}

		if (!anyHit)
		{
			auto& cursor = scene.m_MainCursor;
			auto camera = scene.m_Scene.ActiveCamera();

			auto distance = (cursor->GetPosition() - camera->EyePosition()).Length();
			auto newCursorPosition = ray.m_Origin + distance * ray.m_Direction;

			cursor->SetPosition(newCursorPosition);
		}
	}
}

void MouseBehaviorLayer::OnRightButtonDown(int x, int y)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}
}

void MouseBehaviorLayer::OnMiddleButtonDown(int x, int y)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	m_MoveMouse = true;
}

void MouseBehaviorLayer::OnLeftButtonUp(int x, int y)
{
}

void MouseBehaviorLayer::OnRightButtonUp(int x, int y)
{
}

void MouseBehaviorLayer::OnMiddleButtonUp(int x, int y)
{
	m_MoveMouse = false;
}

void MouseBehaviorLayer::OnScroll(int x, int y, int delta)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (IsMouseOnViewport(x, y))
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene.m_Scene;
		auto camera = scene.ActiveCamera();

		camera->Zoom(-0.03f * delta);
	}
}

void MouseBehaviorLayer::OnMouseMove(int dx, int dy)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_MoveMouse)
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene.m_Scene;
		auto camera = scene.ActiveCamera();

		camera->RotateCamera(0.005 * dx, 0.005 * dy);
	}
}

bool MouseBehaviorLayer::IsMouseOnViewport(int x, int y)
{
	auto& viewport = m_ViewModel.m_ViewportViewModel;
	auto viewportPosX = x - viewport.m_ViewportPosition.x;
	auto viewportPosY = y - viewport.m_ViewportPosition.y;

	return (viewportPosX >= 0 && viewportPosX < viewport.m_ViewportWidth)
		&& (viewportPosY >= 0 && viewportPosY < viewport.m_ViewportHeight);
}

dxm::Vector2 MouseBehaviorLayer::ViewportScreenSpaceCoords(int x, int y)
{
	dxm::Vector2 result = { -1.0f, -1.0f };

	if (IsMouseOnViewport(x, y))
	{
		auto& viewport = m_ViewModel.m_ViewportViewModel;
		auto viewportPosX = x - viewport.m_ViewportPosition.x;
		auto viewportPosY = y - viewport.m_ViewportPosition.y;

		result.x = 2.0f * (float)(viewportPosX) / viewport.m_ViewportWidth - 1.0f;
		result.y = 1.0f - 2.0f * (float)(viewportPosY) / viewport.m_ViewportHeight;
	}

	return result;
}
