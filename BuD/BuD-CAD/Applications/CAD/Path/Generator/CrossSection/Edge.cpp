#include "Edge.h"

Edge::Edge(const IntersectionPoint& plane1, const IntersectionPoint& plane2, const IntersectionPoint& model1, const IntersectionPoint& model2, std::shared_ptr<SceneObjectCAD> intersectedObject)
	: m_PlaneUV1(plane1), m_PlaneUV2(plane2), m_ModelUV1(model1), m_ModelUV2(model2), m_IntersectedObject(intersectedObject)
{
}

auto Edge::InterpolateV(float u) const -> float
{
	if (u < m_PlaneUV1.m_Parameter.x || u > m_PlaneUV2.m_Parameter.x)
	{
		return -1.0f;
	}

	auto t = (u - m_PlaneUV1.m_Parameter.x) / (m_PlaneUV2.m_Parameter.x - m_PlaneUV1.m_Parameter.x);

	return (1 - t) * m_PlaneUV1.m_Parameter.y + t * (m_PlaneUV2.m_Parameter.y);
}
