#include "MillingTool.h"

MillingTool::MillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters)
	: m_MillEntity(scene), m_Parameters(millingToolParameters), m_Position(0.0f, 30.0f, 0.0f)
{
}

void MillingTool::MoveTo(dxm::Vector3 position)
{
	m_Position = position;
}

void MillingTool::MoveBy(dxm::Vector3 difference)
{
	m_Position += difference;
}

dxm::Vector3 MillingTool::Position() const
{
	return m_Position;
}

float MillingTool::Radius() const
{
	return m_Parameters.m_Radius;
}

float MillingTool::Height() const
{
	return m_Parameters.m_Height;
}