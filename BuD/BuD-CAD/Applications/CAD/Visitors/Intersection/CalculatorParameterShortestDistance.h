#pragma once

#include <Applications/CAD/Visitors/AbstractVisitor.h>

class CalculatorParameterShortestDistance : public AbstractVisitor
{
public:
	CalculatorParameterShortestDistance();

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	virtual void Visit(Torus& torus) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

	inline virtual dxm::Vector2 Result() const { return m_Result; }

	virtual void SetParameters(const dxm::Vector2& point1, const dxm::Vector2& point2);

protected:
	dxm::Vector2 m_Point1, m_Point2;
	dxm::Vector2 m_Result;
};
