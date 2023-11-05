#include "VerticalMovementToolMoveValidationLayer.h"

std::optional<ToolMoveValidationError> VerticalMovementToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    const auto& direction = toolCut.m_Direction;

    if (direction.y == 0.0f || toolCut.m_PreviousHeight <= toolCut.m_RequestedHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("Vertical movement inside material block.");
}
