#include "BaseBezierSurface.h"

#include <execution>

BaseBezierSurface::BaseBezierSurface(SceneCAD& scene, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: ParameterizedObject2D(scene.m_Scene), m_SizeU(sizeU), m_SizeV(sizeV), m_Cylinder(cylinder)
{
}

bool BaseBezierSurface::ShouldDisplayPolygon()
{
	return std::all_of(m_BezierPatches.begin(), m_BezierPatches.end(),
		[](std::shared_ptr<BaseBezierPatch> patch)
		{
			return patch->ShouldDisplayPolygon();
		}
	);
}

void BaseBezierSurface::TogglePolygon(bool polygonOn)
{
	std::for_each(std::execution::par, m_BezierPatches.begin(), m_BezierPatches.end(),
		[polygonOn](std::shared_ptr<BaseBezierPatch> patch)
		{
			patch->TogglePolygon(polygonOn);
		}
	);
}

void BaseBezierSurface::UpdateOwnership()
{
	std::weak_ptr<BaseBezierSurface> weakRefToThis = shared_from_this();

	for (auto& patch : m_BezierPatches)
	{
		patch->SetOwnerSurface(weakRefToThis);
	}
}

dxm::Vector3 BaseBezierSurface::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 BaseBezierSurface::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
