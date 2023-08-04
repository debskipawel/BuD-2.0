#pragma once

#include <Scene/SceneObjectCAD.h>

class Torus : public SceneObjectCAD
{
public:
	Torus(BuD::Scene& scene);
	Torus(BuD::Scene& scene, dxm::Vector3 position, dxm::Vector3 orientation);
	Torus(BuD::Scene& scene, dxm::Vector3 position, dxm::Vector3 orientation, float outerRadius, float innerRadius);
	Torus(BuD::Scene& scene, dxm::Vector3 position, dxm::Vector3 orientation, float outerRadius, float innerRadius, unsigned int segmentsU, unsigned int segmentV);

	virtual void Accept(AbstractVisitor& visitor) override;

	struct TorusInstanceData
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Orientation;
		float m_OuterRadius, m_InnerRadius;
		unsigned int m_SegmentsU, m_SegmentsV;
	};

	TorusInstanceData m_InstanceData;
};
