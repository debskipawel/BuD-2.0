#include "ModelMillingToolPathsGenerator.h"

#include <Applications/CAD/Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

#include <Applications/CAD/Path/Generator/RoughSphericalPathGenerator.h>
#include <Applications/CAD/Path/Generator/StandFlatToolPathGenerator.h>
#include <Applications/CAD/Path/Generator/DetailSphericalPathGenerator.h>

#include <Applications/CAD/Path/Converter/PathToGCodeConverter.h>

#include <Applications/CAD/Path/Optimizer/ColinearPathOptimizer.h>

ModelMillingToolPathsGenerator::ModelMillingToolPathsGenerator(SceneCAD& scene)
	: m_Scene(scene)
{
	m_PathOptimizer = std::make_unique<ColinearPathOptimizer>(0.0015f, 1.5f);
}

auto ModelMillingToolPathsGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> std::vector<std::pair<std::string, GCP::GCodeProgram>>
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

	auto converter = PathToGCodeConverter();

	auto gCodeRoughPath = converter.Convert(optimizedRoughPath);
	auto gCodeStandPath = converter.Convert(optimizedStandPath);
	auto gCodeDetailPath = converter.Convert(optimizedDetailPath);

	auto paths = std::vector<std::pair<std::string, GCP::GCodeProgram>>
	{
		{ "rough_path.k16", gCodeRoughPath }, 
		{ "stand_path.f10", gCodeStandPath }, 
		{ "detail_path.k08", gCodeDetailPath },
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
