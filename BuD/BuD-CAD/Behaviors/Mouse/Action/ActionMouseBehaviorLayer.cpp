#include "ActionMouseBehaviorLayer.h"

#include <set>

#include <Visitors/Transform/ApplyTransformVisitor.h>

ActionMouseBehaviorLayer::ActionMouseBehaviorLayer(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayer(dataLayer)
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
	switch (m_MainDataLayer.m_AppStateDataLayer.m_AppState)
	{
		case AppState::MOVE:
		{
			Move(dx, dy);
			break;
		}
		case AppState::ROTATE:
		{
			Rotate(dx, dy);
			break;
		}
		case AppState::SCALE:
		{
			Scale(dx, dy);
			break;
		}
		default:
		{
			break;
		}
	}
}

void ActionMouseBehaviorLayer::Move(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& actionList = m_MainDataLayer.m_SceneDataLayer.m_ActionList;
	auto& groupTransform = actionList.m_GroupTransform;

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

		ApplyMouseAction();
	}
}

void ActionMouseBehaviorLayer::Rotate(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& actionList = m_MainDataLayer.m_SceneDataLayer.m_ActionList;
	auto& groupTransform = actionList.m_GroupTransform;

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

		ApplyMouseAction();
	}
}

void ActionMouseBehaviorLayer::Scale(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& actionList = m_MainDataLayer.m_SceneDataLayer.m_ActionList;
	auto& groupTransform = actionList.m_GroupTransform;

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

		ApplyMouseAction();
	}
}

void ActionMouseBehaviorLayer::ApplyMouseAction()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;
	auto& actionList = m_MainDataLayer.m_SceneDataLayer.m_ActionList;
	auto& groupTransform = actionList.m_GroupTransform;

	auto cursorPosition = groupTransform.m_Position;

	auto lastAction = m_MainDataLayer.m_SceneDataLayer.m_ActionList.Last();
	auto lastActionShared = lastAction.lock();

	if (lastActionShared)
	{
		cursorPosition += lastActionShared->m_Centroid;
	}

	auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor;
	cursor->SetPosition(cursorPosition);
	// ---------------------------------------------

	for (auto& id : selectedGroup.m_SelectedObjects)
	{
		auto& object = scene.m_ObjectList[id];
		auto& initialTransform = selectedGroup.m_InitialTransformCopies[id];

		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ApplyTransformVisitor>(initialTransform, groupTransform, lastActionShared->m_Centroid);
		visitor->Visit(object);
	}
}
