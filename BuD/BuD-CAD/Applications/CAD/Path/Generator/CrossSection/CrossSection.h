#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Path/Generator/CrossSection/Edge.h>

class CrossSection
{
public:
	CrossSection(std::weak_ptr<SceneObjectCAD> plane, std::vector<std::shared_ptr<SceneObjectCAD>> model);
	CrossSection(std::weak_ptr<SceneObjectCAD> plane, std::vector<std::shared_ptr<SceneObjectCAD>> model, std::vector<dxm::Vector2> intersectionStartingParameters);

	virtual auto UpperBound() -> std::vector<dxm::Vector2>;
	virtual auto LowerBound() -> std::vector<dxm::Vector2>;

protected:

	std::vector<Edge> m_IntersectionEdges;
};
