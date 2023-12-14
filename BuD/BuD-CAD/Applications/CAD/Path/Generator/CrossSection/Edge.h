#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

struct Edge
{
	Edge(const IntersectionPoint& plane1, const IntersectionPoint& plane2, const IntersectionPoint& model1, const IntersectionPoint& model2, std::weak_ptr<SceneObjectCAD> intersectedObject);

	virtual auto InterpolateV(float u) const -> float;

	constexpr inline virtual auto StartU() const -> float { return m_PlaneUV1.m_Parameter.x; }
	constexpr inline virtual auto StartV() const -> float { return m_PlaneUV1.m_Parameter.y; }

	constexpr inline virtual auto EndU() const -> float { return m_PlaneUV2.m_Parameter.x; }
	constexpr inline virtual auto EndV() const -> float { return m_PlaneUV2.m_Parameter.y; }

	IntersectionPoint m_PlaneUV1, m_PlaneUV2;
	IntersectionPoint m_ModelUV1, m_ModelUV2;

	std::weak_ptr<SceneObjectCAD> m_IntersectedObject;
};
