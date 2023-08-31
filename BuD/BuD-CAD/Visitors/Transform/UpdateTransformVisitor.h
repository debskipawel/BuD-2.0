#pragma once

#include <Visitors/AbstractVisitor.h>

class UpdateTransformVisitor : public AbstractVisitor
{
public:
	UpdateTransformVisitor() = default;

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	std::set<uint32_t> m_ModifiedObjects;
};
