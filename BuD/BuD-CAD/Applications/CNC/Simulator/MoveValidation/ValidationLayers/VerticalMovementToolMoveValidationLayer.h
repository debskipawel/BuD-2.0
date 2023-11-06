#pragma once

#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/AbstractToolMoveValidationLayer.h>

class VerticalMovementToolMoveValidationLayer : public AbstractToolMoveValidationLayer
{
public:
	VerticalMovementToolMoveValidationLayer(MaterialBlockParameters materialParameters);

	virtual std::optional<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut) override;
};
