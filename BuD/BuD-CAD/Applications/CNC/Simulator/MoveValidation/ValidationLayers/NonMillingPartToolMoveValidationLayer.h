#pragma once

#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/AbstractToolMoveValidationLayer.h>

class NonMillingPartToolMoveValidationLayer : public AbstractToolMoveValidationLayer
{
public:
	NonMillingPartToolMoveValidationLayer(MaterialBlockParameters materialParameters);

	virtual std::optional<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut) override;
};
