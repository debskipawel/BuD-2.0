#include "OffsetSurface.h"

#include <Applications/CAD/Visitors/AbstractVisitor.h>

OffsetSurface::OffsetSurface(BuD::Scene& scene, std::weak_ptr<SceneObjectCAD> internalSurface, float offsetValue)
	: SceneObjectCAD(scene), m_InternalSurface(internalSurface), m_OffsetValue(offsetValue)
{
}

void OffsetSurface::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}
