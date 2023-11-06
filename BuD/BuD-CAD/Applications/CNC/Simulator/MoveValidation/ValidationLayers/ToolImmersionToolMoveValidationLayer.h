#pragma once

#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/AbstractToolMoveValidationLayer.h>

class ToolImmersionToolMoveValidationLayer : public AbstractToolMoveValidationLayer
{
public:
	ToolImmersionToolMoveValidationLayer(MaterialBlockParameters materialParameters);

	virtual std::optional<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut) override;
};
