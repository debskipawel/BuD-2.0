#include "MouseBehaviorLayer.h"

#include <Raycasting/RayIntersectionVisitor.h>

MouseBehaviorLayer::MouseBehaviorLayer(MainViewModel& viewModel)
	: m_ViewModel(viewModel)
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
		HandleSelection(x, y);
	}
	else
	{
		HandleActionStart();
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

	m_ViewModel.m_AppStateViewModel.m_CameraRotating = true;
}

void MouseBehaviorLayer::OnLeftButtonUp(int x, int y)
{
	HandleActionEnd();
}

void MouseBehaviorLayer::OnRightButtonUp(int x, int y)
{
}

void MouseBehaviorLayer::OnMiddleButtonUp(int x, int y)
{
	m_ViewModel.m_AppStateViewModel.m_CameraRotating = false;
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

	if (m_ViewModel.m_AppStateViewModel.m_CameraRotating)
	{
		if (m_ViewModel.m_AppStateViewModel.m_CameraReadyToMove)
		{
			HandleCameraMove(dx, dy);
		}
		else
		{
			HandleCameraRotate(dx, dy);
		}
	}

	if (m_ViewModel.m_AppStateViewModel.m_InAction)
	{
		HandleActionPerform(dx, dy);
	}
}

void MouseBehaviorLayer::HandleSelection(int x, int y)
{
	if (!IsMouseOnViewport(x, y))
	{
		return;
	}

	auto screenSpace = ViewportScreenSpaceCoords(x, y);
	auto ray = m_RayFactory->CreateRay(screenSpace);

	auto closestObject = GetClosestIntersecting(ray);

	if (closestObject)
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

		if (!m_ViewModel.m_AppStateViewModel.m_MultiselectOn)
		{
			scene.m_SelectedGroup.Clear();
		}

		scene.m_SelectedGroup.Add(closestObject);
	}
	else
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

		scene.m_SelectedGroup.Clear();

		MoveCursorAlong(ray);
	}
}

void MouseBehaviorLayer::HandleActionStart()
{
	m_ViewModel.m_AppStateViewModel.m_InAction = !m_ViewModel.m_AppStateViewModel.m_CameraRotating;
}

void MouseBehaviorLayer::HandleActionEnd()
{
	m_ViewModel.m_AppStateViewModel.m_InAction = false;
}

void MouseBehaviorLayer::HandleActionPerform(int dx, int dy)
{
	// TODO: logic for transforming selected objects
}

void MouseBehaviorLayer::HandleCameraRotate(int dx, int dy)
{
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene.m_Scene;
	auto camera = scene.ActiveCamera();

	camera->RotateCamera(0.005 * dx, 0.005 * dy);
}

void MouseBehaviorLayer::HandleCameraMove(int dx, int dy)
{
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene.m_Scene;
	auto camera = scene.ActiveCamera();

	auto right = camera->Right();
	auto up = camera->Up();

	auto direction = -0.05f * (dx * right - dy * up);

	camera->Move(direction);
}

std::shared_ptr<SceneObjectCAD> MouseBehaviorLayer::GetClosestIntersecting(const Ray& ray)
{
	std::shared_ptr<SceneObjectCAD> closestObject = nullptr;

	auto visitor = std::make_unique<RayIntersectionVisitor>(ray);
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

	float minDistance = FLT_MAX;

	for (auto& [id, object] : scene.m_ObjectList)
	{
		visitor->Visit(*object.get());
		auto results = visitor->GetLatestResults();

		if (results.m_Hit && results.m_Distance < minDistance)
		{
			closestObject = object;
			minDistance = results.m_Distance;
		}
	}

	return closestObject;
}

std::vector<std::shared_ptr<SceneObjectCAD>> MouseBehaviorLayer::GetAllIntersecting(const Ray& ray)
{
	auto objectsIntersecting = std::vector<std::shared_ptr<SceneObjectCAD>>();

	auto visitor = std::make_unique<RayIntersectionVisitor>(ray);
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

	for (auto& [id, object] : scene.m_ObjectList)
	{
		visitor->Visit(*object.get());
		auto results = visitor->GetLatestResults();

		if (results.m_Hit)
		{
			objectsIntersecting.push_back(object);
		}
	}

	return objectsIntersecting;
}

void MouseBehaviorLayer::MoveCursorAlong(const Ray& ray)
{
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

	auto& cursor = scene.m_MainCursor;
	auto camera = scene.m_Scene.ActiveCamera();

	auto distance = (cursor->GetPosition() - camera->EyePosition()).Length();
	auto newCursorPosition = ray.m_Origin + distance * ray.m_Direction;

	cursor->SetPosition(newCursorPosition);
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
