#include "ObjectUnselectVisitor.h"

ObjectUnselectVisitor::ObjectUnselectVisitor(SceneDataLayer& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
{
}

void ObjectUnselectVisitor::Visit(Torus& torus)
{
	torus.m_Selected = false;

	m_SceneDataLayer.m_SelectedGroup.Remove(m_Caller);
}

void ObjectUnselectVisitor::Visit(Point& point)
{
	point.m_Selected = false;

	m_SceneDataLayer.m_SelectedGroup.Remove(m_Caller);
}
