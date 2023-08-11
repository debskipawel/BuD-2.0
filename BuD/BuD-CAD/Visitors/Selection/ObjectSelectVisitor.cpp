#include "ObjectSelectVisitor.h"

ObjectSelectVisitor::ObjectSelectVisitor(SceneDataLayer& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
{
}

void ObjectSelectVisitor::Visit(Torus& torus)
{
	torus.m_Selected = true;

	m_SceneDataLayer.m_SelectedGroup.Add(m_Caller);
}

void ObjectSelectVisitor::Visit(Point& point)
{
	point.m_Selected = true;

	m_SceneDataLayer.m_SelectedGroup.Add(m_Caller);
}
