#pragma once

#include <BuD.h>

#include <AppState.h>
#include <AxisLock.h>

class Cursor
{
public:
	explicit Cursor(BuD::Scene& scene);

	dxm::Vector3 GetPosition() const;
	void SetPosition(const dxm::Vector3& position);

	void SetAppState(AppState state);
	void SetAxisLock(AxisLock axisLock);

protected:
	struct CursorInstanceData
	{
		dxm::Vector3 m_Position = dxm::Vector3::Zero;
		AppState m_AppState = AppState::IDLE;
		AxisLock m_AxisLock = AxisLock::NONE;
	};

	CursorInstanceData m_InstanceData;
	BuD::SceneEntity m_SceneEntity;
};
