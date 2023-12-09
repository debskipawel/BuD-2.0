#pragma once

#include <Applications/CAD/Objects/Plane.h>

class InfinitePlane : public Plane
{
public:
	InfinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal);
	InfinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& dU, const dxm::Vector3& dV);

	virtual auto Accept(AbstractVisitor& visitor) -> void override;

	virtual auto GetPoint(float u, float v) const -> dxm::Vector3;

	struct InfinitePlaneInstanceData
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Normal;
	};

	InfinitePlaneInstanceData m_InstanceData;

protected:
	virtual auto UpdateInstanceData() -> void;

	virtual auto InitializeRenderComponent() -> void;
};
