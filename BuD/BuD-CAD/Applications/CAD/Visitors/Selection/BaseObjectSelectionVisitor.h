#pragma once

#include <Applications/CAD/DataLayers/SceneDataLayer.h>
#include <Applications/CAD/Visitors/AbstractVisitor.h>

class BaseObjectSelectionVisitor : public AbstractVisitor
{
public:
	BaseObjectSelectionVisitor(SceneDataLayer& sceneDataLayer);

protected:
	virtual void SetCursorToCentroid();

	SceneDataLayer& m_SceneDataLayer;
};
