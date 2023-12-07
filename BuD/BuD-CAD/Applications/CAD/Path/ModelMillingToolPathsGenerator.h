#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Path/MaterialBlockDetails.h>
#include <Applications/CAD/Path/MillingToolPath.h>

class ModelMillingToolPathsGenerator
{
public:
	ModelMillingToolPathsGenerator(SceneCAD& scene);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> std::vector<std::pair<std::string, MillingToolPath>>;

protected:
	virtual auto GetSurfacesOnScene() -> std::vector<std::weak_ptr<SceneObjectCAD>>;

	SceneCAD& m_Scene;
};
