#include "AnyContactToolMoveValidationLayer.h"

AnyContactToolMoveValidationLayer::AnyContactToolMoveValidationLayer()
{
}

std::optional<ToolMoveValidationError> AnyContactToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    if (toolCut.m_RequestedHeight > toolCut.m_PreviousHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("Contact with material detected");
}
