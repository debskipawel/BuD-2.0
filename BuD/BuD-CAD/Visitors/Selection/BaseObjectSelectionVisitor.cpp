#include "BaseObjectSelectionVisitor.h"

#include <Visitors/Transform/ApplyGroupTransformVisitor.h>

BaseObjectSelectionVisitor::BaseObjectSelectionVisitor(SceneDataLayer& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
{
}

void BaseObjectSelectionVisitor::SetCursorToCentroid()
{
	auto& cursor = m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
	auto centroid = m_SceneDataLayer.m_SelectedForTransform.Centroid();

	cursor->SetPosition(centroid);
}
