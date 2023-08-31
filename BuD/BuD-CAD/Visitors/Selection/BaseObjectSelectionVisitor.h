#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class BaseObjectSelectionVisitor : public AbstractVisitor
{
public:
	BaseObjectSelectionVisitor(SceneDataLayer& sceneDataLayer);

protected:
	virtual void SetCursorToCentroid();

	SceneDataLayer& m_SceneDataLayer;
};
