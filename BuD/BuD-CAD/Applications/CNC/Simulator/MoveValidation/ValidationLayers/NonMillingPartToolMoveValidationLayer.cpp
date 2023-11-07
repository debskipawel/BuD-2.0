#include "NonMillingPartToolMoveValidationLayer.h"

NonMillingPartToolMoveValidationLayer::NonMillingPartToolMoveValidationLayer(MaterialBlockParameters materialParameters)
    : AbstractToolMoveValidationLayer(materialParameters)
{
}

std::optional<ToolMoveValidationError> NonMillingPartToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    const auto& tool = toolCut.m_Tool;

    auto millingToolImmersion = toolCut.m_PreviousHeight - (toolCut.m_RequestedHeight + toolCut.m_PointOnTool.y);
    
    auto millingToolHeight = tool->Height();

    if (millingToolImmersion < millingToolHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("[CNC] Milling using non-milling part detected.");
}
