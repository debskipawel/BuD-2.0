#pragma once

#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/AbstractToolMoveValidationLayer.h>

class AnyContactToolMoveValidationLayer : public AbstractToolMoveValidationLayer
{
public:
	AnyContactToolMoveValidationLayer(MaterialBlockParameters materialParameters);

	virtual std::optional<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut) override;
};
