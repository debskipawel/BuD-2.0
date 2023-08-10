#include "SelectionMouseBehaviorLayer.h"

#include <Raycasting/RayIntersectionVisitor.h>

SelectionMouseBehaviorLayer::SelectionMouseBehaviorLayer(MainViewModel& viewModel)
	: BaseMouseBehaviorLayer(viewModel)
{
}

void SelectionMouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::IDLE)
	{
		HandleSelection(x, y);
	}
}

void SelectionMouseBehaviorLayer::HandleSelection(int x, int y)
{
	if (!IsMouseOnViewport(x, y))
	{
		return;
	}

	auto screenSpace = ViewportScreenSpaceCoords(x, y);
	
	auto rayFactory = RayFactory(m_ViewModel.m_ViewportViewModel, m_ViewModel.m_ObjectListViewModel);
	auto ray = rayFactory.CreateRay(screenSpace);

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

void SelectionMouseBehaviorLayer::MoveCursorAlong(const Ray& ray)
{
	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

	auto& cursor = scene.m_MainCursor;
	auto camera = scene.m_Scene.ActiveCamera();

	auto distance = (cursor->GetPosition() - camera->EyePosition()).Length();
	auto newCursorPosition = ray.m_Origin + distance * ray.m_Direction;

	cursor->SetPosition(newCursorPosition);
}

std::shared_ptr<SceneObjectCAD> SelectionMouseBehaviorLayer::GetClosestIntersecting(const Ray& ray)
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

std::vector<std::shared_ptr<SceneObjectCAD>> SelectionMouseBehaviorLayer::GetAllIntersecting(const Ray& ray)
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
