#include "ToolImmersionToolMoveValidationLayer.h"

ToolImmersionToolMoveValidationLayer::ToolImmersionToolMoveValidationLayer(MaterialBlockParameters materialParameters)
    : AbstractToolMoveValidationLayer(materialParameters)
{
}

std::optional<ToolMoveValidationError> ToolImmersionToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    auto minimumHeight = 0.2f * m_MaterialParameters.m_Size.y;

    if (toolCut.m_RequestedHeight >= minimumHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("Milling tool immersed too deeply into material");
}
