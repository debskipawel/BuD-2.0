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
	auto diff = dxm::Vector2{ m_Point2.x - m_Point1.x, m_Point2.y - m_Point1.y };

	if (abs(diff.x) > 0.5f)
	{
		m_Result.x = diff.x > 0.0f ? diff.x - 1.0f : 1.0f + diff.x;
	}
	else
	{
		m_Result.x = diff.x;
	}

	if (abs(diff.y) > 0.5f)
	{
		m_Result.y = diff.y > 0.0f ? diff.y - 1.0f : 1.0f + diff.y;
	}
	else
	{
		m_Result.y = diff.y;
	}
}

void CalculatorParameterShortestDistance::Visit(InfinitePlane& plane)
{
	m_Result = m_Point2 - m_Point1;
}

void CalculatorParameterShortestDistance::Visit(BezierSurfaceC0& surface)
{
	auto diff = dxm::Vector2{ m_Point2.x - m_Point1.x, m_Point2.y - m_Point1.y };

	if (surface.m_Cylinder && abs(diff.x) > 0.5f)
	{
		m_Result.x = diff.x > 0.0f ? diff.x - 1.0f : 1.0f + diff.x;
	}
	else
	{
		m_Result.x = diff.x;
	}

	m_Result.y = diff.y;
}

void CalculatorParameterShortestDistance::Visit(BezierSurfaceC2& surface)
{
	auto diff = dxm::Vector2{ m_Point2.x - m_Point1.x, m_Point2.y - m_Point1.y };

	if (surface.m_Cylinder && abs(diff.x) > 0.5f)
	{
		m_Result.x = diff.x > 0.0f ? diff.x - 1.0f : 1.0f + diff.x;
	}
	else
	{
		m_Result.x = diff.x;
	}

	m_Result.y = diff.y;
}

void CalculatorParameterShortestDistance::SetParameters(const dxm::Vector2& point1, const dxm::Vector2& point2)
{
	m_Point1 = point1;
	m_Point2 = point2;
}
