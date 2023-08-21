#include "BaseBezierSurface.h"

#include <execution>

BaseBezierSurface::BaseBezierSurface(BuD::Scene& scene, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: PointBasedObjectCAD(scene, {}), m_SizeU(sizeU), m_SizeV(sizeV), m_Cylinder(cylinder)
{
}

bool BaseBezierSurface::ShouldDisplayPolygon()
{
	return std::all_of(m_BezierPatches.begin(), m_BezierPatches.end(),
		[](std::weak_ptr<BaseBezierPatch> patch)
		{
			auto patchShared = patch.lock();

			return patchShared && patchShared->ShouldDisplayPolygon();
		}
	);
}

void BaseBezierSurface::TogglePolygon(bool polygonOn)
{
	std::for_each(std::execution::par, m_BezierPatches.begin(), m_BezierPatches.end(),
		[polygonOn](std::weak_ptr<BaseBezierPatch> patch)
		{
			auto patchShared = patch.lock();

			if (patchShared)
			{
				patchShared->TogglePolygon(polygonOn);
			}
		}
	);
}

dxm::Vector3 BaseBezierSurface::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 BaseBezierSurface::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
