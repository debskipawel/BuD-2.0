#include "AnyContactToolMoveValidationLayer.h"

AnyContactToolMoveValidationLayer::AnyContactToolMoveValidationLayer(MaterialBlockParameters materialParameters)
    : AbstractToolMoveValidationLayer(materialParameters)
{
}

std::optional<ToolMoveValidationError> AnyContactToolMoveValidationLayer::ValidateMove(const ToolCut& toolCut)
{
    if (toolCut.m_RequestedHeight > toolCut.m_PreviousHeight)
    {
        return std::nullopt;
    }

    return ToolMoveValidationError("[CNC] Contact with material detected");
}
