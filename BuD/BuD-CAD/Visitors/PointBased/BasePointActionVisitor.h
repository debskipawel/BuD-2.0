#pragma once

#include <DataLayers/MainDataLayer.h>

#include <Visitors/AbstractVisitor.h>

class BasePointActionVisitor : public AbstractVisitor
{
public:
	BasePointActionVisitor(SceneDataLayer& dataLayer, std::weak_ptr<Point> point);

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

protected:
	virtual void UpdateCentroidCursor();

	SceneDataLayer& m_SceneDataLayer;
	std::weak_ptr<Point> m_Point;
};
