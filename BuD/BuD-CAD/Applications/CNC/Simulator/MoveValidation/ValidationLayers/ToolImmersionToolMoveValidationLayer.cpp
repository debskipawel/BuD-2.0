#include "ToolImmersionToolMoveValidationLayer.h"

std::optional<ToolMoveValidationError> ToolImmersionToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    // TODO: get the actual minimal height from material
    auto minimumHeight = 1.0f;

    if (toolCut.m_RequestedHeight >= minimumHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("Milling tool immersed too deeply into material");
}
