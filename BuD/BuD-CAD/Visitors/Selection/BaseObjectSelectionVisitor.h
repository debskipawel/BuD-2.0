#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class BaseObjectSelectionVisitor : public AbstractVisitor
{
public:
	BaseObjectSelectionVisitor(SceneDataLayer& sceneDataLayer);

protected:
	void ApplyAction(std::shared_ptr<Action> action);

	SceneDataLayer& m_SceneDataLayer;
};
