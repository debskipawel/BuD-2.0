#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>

class Plane : public SceneObjectCAD
{
public:
	Plane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal);
	Plane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& derivativeU, const dxm::Vector3& derivativeV);

	virtual auto GetU() const -> dxm::Vector3 { return m_VectorU; }
	virtual auto GetV() const -> dxm::Vector3 { return m_VectorV; }
	virtual auto GetNormal() const -> dxm::Vector3 { return m_Normal; }

	virtual auto GetPoint(float u, float v) const->dxm::Vector3 = 0;

protected:
	dxm::Vector3 m_Normal;
	dxm::Vector3 m_VectorU, m_VectorV;
};
