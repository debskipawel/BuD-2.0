#pragma once

#include <Visitors/AbstractVisitor.h>

class AfterUpdateTransformVisitor : public AbstractVisitor
{
public:
	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	std::set<uint32_t> m_ModifiedObjects;
};
