#pragma once

#include <Applications/CAD/Objects/Plane.h>

class FinitePlane : public Plane
{
public:
	FinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal, float widthU, float widthV);
	FinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& dU, const dxm::Vector3& dV, float widthU, float widthV);

	virtual auto Accept(AbstractVisitor& visitor) -> void override;

	virtual auto GetPoint(float u, float v) const->dxm::Vector3;

	virtual auto GetTransformedU() const -> dxm::Vector3 { return m_InstanceData.m_VectorU; }
	virtual auto GetTransformedV() const -> dxm::Vector3 { return m_InstanceData.m_VectorV; }

	struct FinitePlaneInstanceData
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_VectorU;
		dxm::Vector3 m_VectorV;
	};

	FinitePlaneInstanceData m_InstanceData;

protected:
	virtual auto InitializeRenderComponent() -> void;

	virtual auto UpdateInstanceData() -> void;
};
