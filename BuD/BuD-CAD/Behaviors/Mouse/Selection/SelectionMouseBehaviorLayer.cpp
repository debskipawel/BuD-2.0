#include "SelectionMouseBehaviorLayer.h"

#include <Visitors/RayIntersection/RayIntersectionVisitor.h>
#include <Visitors/Selection/ObjectSelectVisitor.h>
#include <Visitors/Selection/ObjectUnselectVisitor.h>

SelectionMouseBehaviorLayer::SelectionMouseBehaviorLayer(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayer(dataLayer)
{
}

void SelectionMouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::IDLE)
	{
		HandleSelection(x, y);
	}
}

void SelectionMouseBehaviorLayer::ClearSelected()
{
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected;

	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectUnselectVisitor>(m_MainDataLayer.m_SceneDataLayer);

	while (selectedGroup.Count())
	{
		auto object = selectedGroup.First();

		visitor->Visit(object);
	}
}

void SelectionMouseBehaviorLayer::HandleSelection(int x, int y)
{
	if (!m_MainDataLayer.m_ViewportDataLayer.IsMouseOnViewport(x, y))
	{
		return;
	}

	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto camera = scene.m_Scene.ActiveCamera();
	auto screenSpace = ViewportScreenSpaceCoords(x, y);
	
	auto rayFactory = RayFactory(camera);
	auto ray = rayFactory.CreateRay(screenSpace);

	auto closestObject = GetClosestIntersecting(ray);
	auto closestObjectShared = closestObject.lock();

	if (closestObjectShared)
	{
		if (!m_MainDataLayer.m_AppStateDataLayer.m_MultiselectOn)
		{
			ClearSelected();
		}
		else if (m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.Selected(closestObjectShared->Id()))
		{
			std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectUnselectVisitor>(m_MainDataLayer.m_SceneDataLayer);
			visitor->Visit(closestObject);
			
			return;
		}

		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectSelectVisitor>(m_MainDataLayer.m_SceneDataLayer);
		visitor->Visit(closestObject);
	}
	else
	{
		ClearSelected();

		MoveCursorAlong(ray);
	}
}

void SelectionMouseBehaviorLayer::MoveCursorAlong(const Ray& ray)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

	auto& cursor = scene.m_MainCursor;
	auto camera = scene.m_Scene.ActiveCamera();

	auto distance = (cursor->GetPosition() - camera->EyePosition()).Length();
	auto newCursorPosition = ray.m_Origin + distance * ray.m_Direction;

	cursor->SetPosition(newCursorPosition);
}

std::weak_ptr<SceneObjectCAD> SelectionMouseBehaviorLayer::GetClosestIntersecting(const Ray& ray)
{
	std::weak_ptr<SceneObjectCAD> closestObject = {};

	auto visitor = std::make_unique<RayIntersectionVisitor>(ray);
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

	float minDistance = FLT_MAX;

	for (auto& [id, object] : scene.m_ObjectList)
	{
		visitor->Visit(object);
		auto results = visitor->GetLatestResults();

		if (results.m_Hit && results.m_Distance < minDistance)
		{
			closestObject = object;
			minDistance = results.m_Distance;
		}
	}

	return closestObject;
}

std::vector<std::weak_ptr<SceneObjectCAD>> SelectionMouseBehaviorLayer::GetAllIntersecting(const Ray& ray)
{
	auto objectsIntersecting = std::vector<std::weak_ptr<SceneObjectCAD>>();

	auto visitor = std::make_unique<RayIntersectionVisitor>(ray);
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

	for (auto& [id, object] : scene.m_ObjectList)
	{
		visitor->Visit(object);
		auto results = visitor->GetLatestResults();

		if (results.m_Hit)
		{
			objectsIntersecting.push_back(object);
		}
	}

	return objectsIntersecting;
}
