#include "ModelMillingToolPathsGenerator.h"

#include <Applications/CAD/Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

#include <Applications/CAD/Path/Generator/RoughSphericalPathGenerator.h>
#include <Applications/CAD/Path/Generator/StandFlatToolPathGenerator.h>
#include <Applications/CAD/Path/Generator/DetailSphericalPathGenerator.h>

#include <Applications/CAD/Path/Optimizer/ColinearPathOptimizer.h>

ModelMillingToolPathsGenerator::ModelMillingToolPathsGenerator(SceneCAD& scene)
	: m_Scene(scene)
{
	m_PathOptimizer = std::make_unique<ColinearPathOptimizer>();
}

auto ModelMillingToolPathsGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> std::vector<std::pair<std::string, MillingToolPath>>
{
	auto modelSurfaces = GetSurfacesOnScene();

	std::unique_ptr<AbstractPathGenerator> roughToolGenerator = std::make_unique<RoughSphericalPathGenerator>(m_Scene, modelSurfaces);
	std::unique_ptr<AbstractPathGenerator> standToolGenerator = std::make_unique<StandFlatToolPathGenerator>(m_Scene, modelSurfaces);
	std::unique_ptr<AbstractPathGenerator> detailToolGenerator = std::make_unique<DetailSphericalPathGenerator>(m_Scene, modelSurfaces);

	auto roughPath = roughToolGenerator->GeneratePaths(materialBlockDetails);
	auto standPath = standToolGenerator->GeneratePaths(materialBlockDetails);
	auto detailPath = detailToolGenerator->GeneratePaths(materialBlockDetails);

	auto optimizedRoughPath = m_PathOptimizer->Optimize(roughPath);
	auto optimizedStandPath = m_PathOptimizer->Optimize(standPath);
	auto optimizedDetailPath = m_PathOptimizer->Optimize(detailPath);

	auto points = std::vector<std::weak_ptr<SceneObjectCAD>>();

	for (const auto& pos : optimizedRoughPath.GetPath())
	{
		points.emplace_back(m_Scene.CreatePoint(pos));
	}

	auto paths = std::vector<std::pair<std::string, MillingToolPath>>
	{
		{ "Rough", optimizedRoughPath }, { "Stand", optimizedStandPath }, { "Detail", optimizedDetailPath },
	};

	return paths;
}

auto ModelMillingToolPathsGenerator::GetSurfacesOnScene() -> std::vector<std::weak_ptr<SceneObjectCAD>>
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
