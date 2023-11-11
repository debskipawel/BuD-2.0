#pragma once

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>

#include <Applications/CAD/Visitors/AbstractVisitor.h>

class BasePointActionVisitor : public AbstractVisitor
{
public:
	BasePointActionVisitor(SceneDataLayerCAD& dataLayer, std::weak_ptr<Point> point);

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

protected:
	virtual void UpdateCentroidCursor();

	SceneDataLayerCAD& m_SceneDataLayer;
	std::weak_ptr<Point> m_Point;
};
