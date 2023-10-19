#include "ActionMouseBehaviorLayer.h"

#include <set>

#include <Applications/CAD/Visitors/Transform/AfterUpdateTransformVisitor.h>
#include <Applications/CAD/Visitors/Transform/ApplyGroupTransformVisitor.h>

ActionMouseBehaviorLayer::ActionMouseBehaviorLayer(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayerCAD(dataLayer)
{
}

void ActionMouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	static std::set<AppState> actionableAppStates = { AppState::MOVE, AppState::ROTATE, AppState::SCALE };

	if (actionableAppStates.contains(m_MainDataLayer.m_AppStateDataLayer.m_AppState) 
		&& m_MainDataLayer.m_ViewportDataLayer.IsMouseOnViewport(x, y)
	)
	{
		HandleActionStart();
	}
}

void ActionMouseBehaviorLayer::OnLeftButtonUp(int x, int y)
{
	HandleActionEnd();
}

void ActionMouseBehaviorLayer::OnMouseMove(int dx, int dy)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_MainDataLayer.m_AppStateDataLayer.m_InAction)
	{
		HandleActionPerform(dx, dy);
	}
}

void ActionMouseBehaviorLayer::HandleActionStart()
{
	m_MainDataLayer.m_AppStateDataLayer.m_InAction = !m_MainDataLayer.m_AppStateDataLayer.m_CameraRotating;
}

void ActionMouseBehaviorLayer::HandleActionEnd()
{
	m_MainDataLayer.m_AppStateDataLayer.m_InAction = false;
}

void ActionMouseBehaviorLayer::HandleActionPerform(int dx, int dy)
{
	std::map<AppState, std::function<void(int, int)>> appStateToActionMap =
	{
		{ AppState::MOVE, [this](int dx, int dy) { Move(dx, dy); } },
		{ AppState::ROTATE, [this](int dx, int dy) { Rotate(dx, dy); } },
		{ AppState::SCALE, [this](int dx, int dy) { Scale(dx, dy); } },
	};

	auto& handler = appStateToActionMap.at(m_MainDataLayer.m_AppStateDataLayer.m_AppState);
	handler(dx, dy);
}

void ActionMouseBehaviorLayer::Move(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& selectedForTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	auto sensitivity = 0.01f;

	auto camera = scene.m_Scene.ActiveCamera();
	auto moveVector = sensitivity * (dx * camera->Right() - dy * camera->Up());

	switch (m_MainDataLayer.m_AppStateDataLayer.m_AxisLock)
	{
		case AxisLock::X:
		{
			moveVector.y = 0.0f;
			moveVector.z = 0.0f;
			break;
		}
		case AxisLock::Y:
		{
			moveVector.x = 0.0f;
			moveVector.z = 0.0f;
			break;
		}
		case AxisLock::Z:
		{
			moveVector.y = 0.0f;
			moveVector.x = 0.0f;
			break;
		}
		default:
		{
			break;
		}
	}

	if (moveVector.LengthSquared())
	{
		groupTransform.m_Position += moveVector;

		ApplyGroupMouseAction();
	}
}

void ActionMouseBehaviorLayer::Rotate(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& selectedForTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	auto camera = scene.m_Scene.ActiveCamera();

	float rightAngle = -static_cast<float>(dy);
	float upAngle = static_cast<float>(dx);

	auto rotationMtx = dxm::Matrix::CreateFromAxisAngle(camera->Up(), upAngle) * dxm::Matrix::CreateFromAxisAngle(camera->Right(), rightAngle);
	auto rotationVector = (dx * camera->Right() - dy * camera->Up());

	switch (m_MainDataLayer.m_AppStateDataLayer.m_AxisLock)
	{
		case AxisLock::X:
		{
			rotationVector.y = 0.0f;
			rotationVector.z = 0.0f;
			break;
		}
		case AxisLock::Y:
		{
			rotationVector.x = 0.0f;
			rotationVector.z = 0.0f;
			break;
		}
		case AxisLock::Z:
		{
			rotationVector.y = 0.0f;
			rotationVector.x = 0.0f;
			break;
		}
		default:
		{
			break;
		}
	}

	if (rotationVector.LengthSquared())
	{
		groupTransform.m_Rotation += rotationVector;

		ApplyGroupMouseAction();
	}
}

void ActionMouseBehaviorLayer::Scale(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& selectedForTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	auto sensitivity = 0.003;

	auto camera = scene.m_Scene.ActiveCamera();
	auto scaleVector = sensitivity * (dx * camera->Right() - dy * camera->Up());

	switch (m_MainDataLayer.m_AppStateDataLayer.m_AxisLock)
	{
		case AxisLock::X:
		{
			scaleVector.y = 0.0f;
			scaleVector.z = 0.0f;
			break;
		}
		case AxisLock::Y:
		{
			scaleVector.x = 0.0f;
			scaleVector.z = 0.0f;
			break;
		}
		case AxisLock::Z:
		{
			scaleVector.y = 0.0f;
			scaleVector.x = 0.0f;
			break;
		}
		default:
		{
			break;
		}
	}

	if (scaleVector.LengthSquared())
	{
		groupTransform.m_Scale += scaleVector;

		ApplyGroupMouseAction();
	}
}

void ActionMouseBehaviorLayer::ApplyGroupMouseAction()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& selectedForTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;
	auto& groupTransform = selectedForTransform.GroupTransform();

	auto centroid = selectedForTransform.Centroid();

	auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
	cursor->SetPosition(centroid);

	auto onTransformVisitor = std::make_unique<ApplyGroupTransformVisitor>(groupTransform, centroid - groupTransform.m_Position);
	std::unique_ptr<AbstractVisitor> afterTransformVisitor = std::make_unique<AfterUpdateTransformVisitor>();

	selectedForTransform.ForEachSelected(
		[&selectedForTransform, &onTransformVisitor](std::shared_ptr<SceneObjectCAD> object)
		{
			auto initialTransform = selectedForTransform.InitialTransform(object->Id());

			onTransformVisitor->SetInitialTransform(initialTransform);
			onTransformVisitor->Visit(object);
		});

	selectedForTransform.ForEachSelected(
		[&afterTransformVisitor](std::shared_ptr<SceneObjectCAD> object)
		{
			afterTransformVisitor->Visit(object);
		}
	);
}
