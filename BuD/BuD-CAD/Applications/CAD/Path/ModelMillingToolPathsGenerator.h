#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Path/Optimizer/AbstractPathOptimizer.h>
#include <Applications/CAD/Path/MaterialBlockDetails.h>

#include <GCodeProgram.h>

class ModelMillingToolPathsGenerator
{
public:
	ModelMillingToolPathsGenerator(SceneCAD& scene);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> std::vector<std::pair<std::string, GCP::GCodeProgram>>;

protected:
	virtual auto GetSurfacesOnScene() -> std::vector<std::weak_ptr<SceneObjectCAD>>;

	SceneCAD& m_Scene;

	std::unique_ptr<AbstractPathOptimizer> m_PathOptimizer;
};
