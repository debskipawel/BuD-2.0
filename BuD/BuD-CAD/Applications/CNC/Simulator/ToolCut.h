#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>

struct ToolCut
{
	std::shared_ptr<MillingTool> m_Tool;

	dxm::Vector3 m_Direction;
	dxm::Vector3 m_PointOnTool;

	float m_PreviousHeight;
	float m_RequestedHeight;
};
