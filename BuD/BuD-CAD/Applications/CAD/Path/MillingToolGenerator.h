#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Path/MaterialBlockDetails.h>
#include <Applications/CAD/Path/MillingToolPath.h>

class MillingToolGenerator
{
public:
	MillingToolGenerator(SceneCAD& scene, const MaterialBlockDetails& materialBlockDetails);

	virtual auto GeneratePaths() -> std::vector<std::pair<std::string, MillingToolPath>>;

protected:
	virtual auto GetSurfacesOnScene() -> std::vector<std::weak_ptr<SceneObjectCAD>>;

	SceneCAD& m_Scene;

	MaterialBlockDetails m_MaterialBlockDetails;
};
