#include "ParameterWrapperVisitor.h"

void ParameterWrapperVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	m_ParameterOutOfRange = false;

	m_WrappedU = m_WrappedV = 0;

	AbstractVisitor::Visit(object);
}

void ParameterWrapperVisitor::Visit(Torus& torus)
{
	m_ParameterOutOfRange = false;

	m_WrappedU = m_Parameter.x < 0.0f ? -1.0f : (m_Parameter.x >= 1.0f ? 1.0f : 0.0f);
	m_WrappedV = m_Parameter.y < 0.0f ? -1.0f : (m_Parameter.y >= 1.0f ? 1.0f : 0.0f);

	m_Parameter.x -= floor(m_Parameter.x);
	m_Parameter.y -= floor(m_Parameter.y);
}

void ParameterWrapperVisitor::Visit(FinitePlane& plane)
{
	m_WrappedU = m_WrappedV = 0;

	m_ParameterOutOfRange = m_Parameter.x < 0.0f || m_Parameter.x > 1.0f || m_Parameter.y < 0.0f || m_Parameter.y > 1.0f;
}

void ParameterWrapperVisitor::Visit(BezierSurfaceC0& surface)
{
	m_WrappedU = m_WrappedV = 0;

	if (surface.m_Cylinder)
	{
		m_WrappedU = m_Parameter.x < 0.0f ? -1.0f : (m_Parameter.x >= 1.0f ? 1.0f : 0.0f);

		m_Parameter.x -= floor(m_Parameter.x);
	}

	m_ParameterOutOfRange = m_Parameter.x < 0.0f || m_Parameter.x > 1.0f || m_Parameter.y < 0.0f || m_Parameter.y > 1.0f;

	m_Parameter.x = std::clamp(m_Parameter.x, 0.0f, 1.0f);
	m_Parameter.y = std::clamp(m_Parameter.y, 0.0f, 1.0f);
}

void ParameterWrapperVisitor::Visit(BezierSurfaceC2& surface)
{
	m_WrappedU = m_WrappedV = 0;

	if (surface.m_Cylinder)
	{
		m_WrappedU = m_Parameter.x < 0.0f ? -1.0f : (m_Parameter.x >= 1.0f ? 1.0f : 0.0f);

		m_Parameter.x -= floor(m_Parameter.x);
	}

	m_ParameterOutOfRange = m_Parameter.x < 0.0f || m_Parameter.x > 1.0f || m_Parameter.y < 0.0f || m_Parameter.y > 1.0f;

	m_Parameter.x = std::clamp(m_Parameter.x, 0.0f, 1.0f);
	m_Parameter.y = std::clamp(m_Parameter.y, 0.0f, 1.0f);
}

void ParameterWrapperVisitor::Visit(OffsetSurface& surface)
{
	ParameterWrapperVisitor::Visit(surface.InternalSurface());
}

void ParameterWrapperVisitor::SetParameter(const dxm::Vector2& parameter)
{
	m_Parameter = parameter;
}
