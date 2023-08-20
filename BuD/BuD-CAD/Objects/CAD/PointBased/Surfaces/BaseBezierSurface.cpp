#include "BaseBezierSurface.h"

BaseBezierSurface::BaseBezierSurface(BuD::Scene& scene, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: PointBasedObjectCAD(scene, {}), m_SizeU(sizeU), m_SizeV(sizeV), m_Cylinder(cylinder)
{
}

dxm::Vector3 BaseBezierSurface::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 BaseBezierSurface::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
