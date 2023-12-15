#include "OffsetSurface.h"

#include <Applications/CAD/Visitors/AbstractVisitor.h>

OffsetSurface::OffsetSurface(BuD::Scene& scene, std::weak_ptr<SceneObjectCAD> internalSurface, float offsetValue)
	: SceneObjectCAD(scene), m_InternalSurface(internalSurface), m_OffsetValue(offsetValue)
{
	auto inner = m_InternalSurface.lock();

	m_Tag = std::format("Offset ({}) with offset {}", inner ? inner->m_Tag : "??", m_OffsetValue);
}

void OffsetSurface::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}
