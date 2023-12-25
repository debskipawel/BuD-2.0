#pragma once

#include <Applications/CAD/Path/Generator/AbstractPathGenerator.h>

class DetailSphericalPathGenerator : public AbstractPathGenerator
{
public:
	DetailSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath override;

protected:
	virtual auto GenerateGeneralPathsForDetailMilling(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>;

	virtual auto GeneratePathsForButtonBodyIntersection(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>;
	virtual auto GeneratePathsForScrewButtonIntersection(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>;
	
	virtual auto GeneratePathsForSharpEdges(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>;
	virtual auto GeneratePathsForOffset(const MaterialBlockDetails& materialBlockDetails, std::shared_ptr<OffsetSurface> offset, float minU, float maxU) -> std::vector<dxm::Vector3>;

	virtual auto FindCrossSectionPath(const MaterialBlockDetails& materialBlockDetails, std::vector<std::shared_ptr<SceneObjectCAD>> model, const dxm::Vector3& startPosition, const dxm::Vector3& endPosition) -> std::vector<dxm::Vector3>;

	virtual auto CreateHorizontalStandPlane(const MaterialBlockDetails& materialBlockDetails) -> std::weak_ptr<SceneObjectCAD>;
};
