#include "MillingToolGenerator.h"

#include <Applications/CAD/Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

MillingToolGenerator::MillingToolGenerator(SceneCAD& scene, const MaterialBlockDetails& materialBlockDetails)
	: m_Scene(scene), m_MaterialBlockDetails(materialBlockDetails)
{
}

auto MillingToolGenerator::GeneratePaths() -> void
{
	auto modelSurfaces = GetSurfacesOnScene();

	auto planePosition = dxm::Vector3::Zero + m_MaterialBlockDetails.m_StandHeight * dxm::Vector3::UnitY;
	auto planeWidth = m_MaterialBlockDetails.m_Size.x;
	auto planeLength = m_MaterialBlockDetails.m_Size.z;

	auto materialPlane = m_Scene.CreateFlatBezierSurfaceC0(planePosition, 1, 1, planeWidth, planeLength).lock();

	m_Scene.DeleteObject(*materialPlane);
}

auto MillingToolGenerator::GetSurfacesOnScene() -> std::vector<std::weak_ptr<SceneObjectCAD>>
{
	auto intersectableObjects = std::vector<std::weak_ptr<SceneObjectCAD>>();

	auto& objectList = m_Scene.m_ObjectList;

	std::unique_ptr<BaseValidationVisitor> intersectionEligibilityVisitor = std::make_unique<IntersectionEligibilityValidationVisitor>();

	for (auto& [id, object] : objectList)
	{
		intersectionEligibilityVisitor->Visit(object);

		if (intersectionEligibilityVisitor->Valid())
		{
			intersectableObjects.push_back(object);
		}
	}

	return intersectableObjects;
}
