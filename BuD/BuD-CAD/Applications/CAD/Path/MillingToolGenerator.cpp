#include "MillingToolGenerator.h"

#include <Applications/CAD/Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

#include <Applications/CAD/Path/Generator/RoughSphericalPathGenerator.h>
#include <Applications/CAD/Path/Generator/StandFlatToolPathGenerator.h>
#include <Applications/CAD/Path/Generator/DetailSphericalPathGenerator.h>

MillingToolGenerator::MillingToolGenerator(SceneCAD& scene, const MaterialBlockDetails& materialBlockDetails)
	: m_Scene(scene), m_MaterialBlockDetails(materialBlockDetails)
{
}

auto MillingToolGenerator::GeneratePaths() -> std::vector<std::pair<std::string, MillingToolPath>>
{
	auto modelSurfaces = GetSurfacesOnScene();

	std::unique_ptr<AbstractPathGenerator> roughToolGenerator = std::make_unique<RoughSphericalPathGenerator>(m_Scene, modelSurfaces);
	std::unique_ptr<AbstractPathGenerator> standToolGenerator = std::make_unique<StandFlatToolPathGenerator>(m_Scene, modelSurfaces);
	std::unique_ptr<AbstractPathGenerator> detailToolGenerator = std::make_unique<DetailSphericalPathGenerator>(m_Scene, modelSurfaces);

	auto paths = std::vector<std::pair<std::string, MillingToolPath>>();

	paths.emplace_back(std::make_pair("Rough", roughToolGenerator->GeneratePaths(m_MaterialBlockDetails)));
	paths.emplace_back(std::make_pair("Stand", standToolGenerator->GeneratePaths(m_MaterialBlockDetails)));
	paths.emplace_back(std::make_pair("Detail", detailToolGenerator->GeneratePaths(m_MaterialBlockDetails)));

	return paths;
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
