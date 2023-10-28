#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/Tools/MillingToolParameters.h>

class MillingTool
{
public:
	MillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void EnableRendering() = 0;
	virtual void DisableRendering() = 0;

	virtual void MoveTo(dxm::Vector3 position);
	virtual void MoveBy(dxm::Vector3 difference);

	virtual dxm::Vector3 Position() const;

protected:
	MillingToolParameters m_Parameters;
	dxm::Vector3 m_Position;

	BuD::SceneEntity m_MillEntity;
};
