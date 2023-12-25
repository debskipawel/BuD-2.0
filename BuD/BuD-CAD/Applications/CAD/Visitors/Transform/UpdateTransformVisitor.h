#pragma once

#include <Applications/CAD/Visitors/AbstractVisitor.h>

class UpdateTransformVisitor : public AbstractVisitor
{
public:
	UpdateTransformVisitor() = default;

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(FinitePlane& plane) override;

protected:
	std::set<uint32_t> m_ModifiedObjects;
};
