#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>

class InfinitePlane : public SceneObjectCAD
{
public:
	InfinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal);

	virtual auto Accept(AbstractVisitor& visitor) -> void override;

	virtual auto GetU() const -> dxm::Vector3 { return m_VectorU; }
	virtual auto GetV() const -> dxm::Vector3 { return m_VectorV; }
	virtual auto GetNormal() const -> dxm::Vector3 { return m_Normal; }

	virtual auto GetPoint(float u, float v) const -> dxm::Vector3;

protected:
	dxm::Vector3 m_Normal;
	dxm::Vector3 m_VectorU, m_VectorV;
};
