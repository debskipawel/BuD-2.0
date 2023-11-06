#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/Tools/MillingToolParameters.h>

class MillingToolVisitor;

class MillingTool
{
public:
	MillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void Accept(MillingToolVisitor& visitor) = 0;

	virtual void EnableRendering() = 0;
	virtual void DisableRendering() = 0;

	virtual void MoveTo(dxm::Vector3 position);
	virtual void MoveBy(dxm::Vector3 difference);

	virtual dxm::Vector3 CenterPoint() const = 0;
	virtual dxm::Vector3 Position() const;
	
	virtual float Radius() const;
	virtual float Height() const;

	virtual dxm::Vector3 GetCuttingPointInDirection(float x, float y, dxm::Vector3 direction) = 0;

	virtual float LocalHeight(float x, float y) = 0;
	virtual dxm::Vector3 LocalNormal(float x, float y) = 0;

	MillingToolParameters m_Parameters;

protected:
	dxm::Vector3 m_Position;

	BuD::SceneEntity m_MillEntity;
};