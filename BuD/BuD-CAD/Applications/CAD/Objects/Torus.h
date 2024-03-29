#pragma once

#include <Applications/CAD/Objects/ParameterizedObject2D.h>

class Torus : public ParameterizedObject2D
{
public:
	Torus(BuD::Scene& scene);
	Torus(BuD::Scene& scene, dxm::Vector3 position);
	Torus(BuD::Scene& scene, dxm::Vector3 position, float outerRadius, float innerRadius);
	Torus(BuD::Scene& scene, dxm::Vector3 position, float outerRadius, float innerRadius, unsigned int segmentsU, unsigned int segmentV);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve) override;
	virtual void RemoveIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve) override;

	struct TorusInstanceData
	{
		dxm::Matrix m_ModelMatrix;
		float m_OuterRadius, m_InnerRadius;
		unsigned int m_SegmentsU, m_SegmentsV;
		dxm::Vector3 m_Color;
	};

	TorusInstanceData m_InstanceData;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;

protected:
	void CreateInstancedRenderingPass();
	void CreateTrimmedRenderingPass();

	BuD::RenderingPass m_InstancedRenderingPass;
	BuD::RenderingPass m_TrimmedRenderingPass;
};
