#pragma once

#include <Applications/CAD/DataLayers/SceneDataLayerCAD.h>
#include <Applications/CAD/Visitors/AbstractVisitor.h>

class BaseObjectSelectionVisitor : public AbstractVisitor
{
public:
	BaseObjectSelectionVisitor(SceneDataLayerCAD& sceneDataLayer);

protected:
	virtual void SetCursorToCentroid();

	SceneDataLayerCAD& m_SceneDataLayer;
};
