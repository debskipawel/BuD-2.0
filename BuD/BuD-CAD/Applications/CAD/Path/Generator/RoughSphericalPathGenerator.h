#pragma once

#include <Applications/CAD/Path/Generator/AbstractPathGenerator.h>

class RoughSphericalPathGenerator : public AbstractPathGenerator
{
public:
	RoughSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath override;

protected:
	virtual auto CreateHorizontalPlane(const MaterialBlockDetails& materialBlockDetails, float height) -> std::weak_ptr<SceneObjectCAD>;
	virtual auto GenerateCrossSection(const MaterialBlockDetails& materialBlockDetails, const dxm::Vector3& startPosition, const dxm::Vector3& endPosition, std::weak_ptr<SceneObjectCAD> horizontalPlane) -> std::vector<dxm::Vector3>;
};
