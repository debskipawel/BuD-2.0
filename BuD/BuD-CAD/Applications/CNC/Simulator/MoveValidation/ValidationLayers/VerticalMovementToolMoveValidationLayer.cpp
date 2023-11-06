#include "VerticalMovementToolMoveValidationLayer.h"

VerticalMovementToolMoveValidationLayer::VerticalMovementToolMoveValidationLayer(MaterialBlockParameters materialParameters)
    : AbstractToolMoveValidationLayer(materialParameters)
{
}

std::optional<ToolMoveValidationError> VerticalMovementToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    const auto& direction = toolCut.m_Direction;

    if (abs(direction.y) < 1e-3f || toolCut.m_PreviousHeight <= toolCut.m_RequestedHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("Vertical movement inside material block.");
}
