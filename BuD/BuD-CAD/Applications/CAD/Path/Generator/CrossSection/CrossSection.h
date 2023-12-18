#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Path/Generator/CrossSection/Edge.h>

class CrossSection
{
public:
	CrossSection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model);
	CrossSection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model, const std::vector<dxm::Vector2>& intersectionStartingParameters);

	virtual auto UpperBound() -> std::vector<dxm::Vector2>;
	virtual auto LowerBound() -> std::vector<dxm::Vector2>;

protected:
	virtual auto InitializeIntersectionEdgesCollection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model, const std::vector<dxm::Vector3>& intersectionStartingPoints) -> void;

	virtual auto ConnectDiscontinuousPath(const std::shared_ptr<SceneObjectCAD>& object, std::vector<Edge>& intersectionEdges) -> void;

	std::vector<Edge> m_IntersectionEdges;
};
