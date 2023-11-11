#include "BaseObjectSelectionVisitor.h"

#include <Applications/CAD/Visitors/Transform/ApplyGroupTransformVisitor.h>

BaseObjectSelectionVisitor::BaseObjectSelectionVisitor(SceneDataLayerCAD& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
{
}

void BaseObjectSelectionVisitor::SetCursorToCentroid()
{
	auto& cursor = m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;
	auto centroid = m_SceneDataLayer.m_SelectedForTransform.Centroid();

	cursor->SetPosition(centroid);
}
