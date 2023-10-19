#include "BasePointActionVisitor.h"

BasePointActionVisitor::BasePointActionVisitor(SceneDataLayer& dataLayer, std::weak_ptr<Point> point)
	: m_SceneDataLayer(dataLayer)
{
}

void BasePointActionVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	if (!m_Point.lock())
	{
		return;
	}

	AbstractVisitor::Visit(object);
}

void BasePointActionVisitor::UpdateCentroidCursor()
{
	auto& selectedTransform = m_SceneDataLayer.m_SelectedForTransform;
	auto& cursor = m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

	selectedTransform.Select(m_Point);

	cursor->SetPosition(selectedTransform.Centroid());
}
