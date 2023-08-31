#include "CalculatorParameterShortestDistance.h"

CalculatorParameterShortestDistance::CalculatorParameterShortestDistance()
{
}

void CalculatorParameterShortestDistance::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	m_Result = { NAN, NAN };

	AbstractVisitor::Visit(object);
}

void CalculatorParameterShortestDistance::Visit(Torus& torus)
{
	auto diff = dxm::Vector2{ abs(m_Point2.x - m_Point1.x), abs(m_Point2.y - m_Point1.y) };

	m_Result.x = min(diff.x, 1.0f - diff.x);
	m_Result.y = min(diff.y, 1.0f - diff.y);
}

void CalculatorParameterShortestDistance::Visit(BezierSurfaceC0& surface)
{
	auto diff = dxm::Vector2{ abs(m_Point2.x - m_Point1.x), abs(m_Point2.y - m_Point1.y) };

	m_Result.x = surface.m_Cylinder ? min(diff.x, 1.0f - diff.x) : diff.x;
	m_Result.y = diff.y;
}

void CalculatorParameterShortestDistance::Visit(BezierSurfaceC2& surface)
{
	auto diff = dxm::Vector2{ abs(m_Point2.x - m_Point1.x), abs(m_Point2.y - m_Point1.y) };

	m_Result.x = surface.m_Cylinder ? min(diff.x, 1.0f - diff.x) : diff.x;
	m_Result.y = diff.y;
}

void CalculatorParameterShortestDistance::SetParameters(const dxm::Vector2& point1, const dxm::Vector2& point2)
{
	m_Point1 = point1;
	m_Point2 = point2;
}
