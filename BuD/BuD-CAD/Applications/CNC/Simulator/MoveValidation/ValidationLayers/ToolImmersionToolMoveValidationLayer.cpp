#include "ToolImmersionToolMoveValidationLayer.h"

ToolImmersionToolMoveValidationLayer::ToolImmersionToolMoveValidationLayer(MaterialBlockParameters materialParameters)
    : AbstractToolMoveValidationLayer(materialParameters)
{
}

std::optional<ToolMoveValidationError> ToolImmersionToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    auto minimumHeight = m_MaterialParameters.m_MaxImmersion;

    if (toolCut.m_RequestedHeight >= minimumHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("[CNC] Milling tool immersed too deeply into material");
}
