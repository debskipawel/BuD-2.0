#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Path/Generator/CrossSection/Edge.h>
#include <Applications/CAD/Path/Generator/CrossSection/BoundingPolygon.h>

class CrossSection
{
public:
	CrossSection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model);
	CrossSection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model, const std::vector<dxm::Vector2>& intersectionStartingParameters);

	virtual auto UpperBound(const std::vector<std::shared_ptr<SceneObjectCAD>>& surfacesToAvoid = {}) -> BoundingPolygon;
	virtual auto LowerBound(const std::vector<std::shared_ptr<SceneObjectCAD>>& surfacesToAvoid = {}) -> BoundingPolygon;

protected:
	virtual auto InitializeIntersectionEdgesCollection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model, const std::vector<dxm::Vector3>& intersectionStartingPoints) -> void;

	virtual auto ConnectDiscontinuousPath(const std::shared_ptr<SceneObjectCAD>& object, std::vector<Edge>& intersectionEdges) -> void;

	std::vector<Edge> m_IntersectionEdges;
};
