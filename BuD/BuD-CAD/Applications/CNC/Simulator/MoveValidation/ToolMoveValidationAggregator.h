#pragma once

#include <memory>
#include <vector>

#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/AbstractToolMoveValidationLayer.h>

class ToolMoveValidationAgregator
{
public:
	ToolMoveValidationAgregator(const std::vector<std::shared_ptr<AbstractToolMoveValidationLayer>>& validationLayers);

	virtual std::vector<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut);

protected:
	std::vector<std::shared_ptr<AbstractToolMoveValidationLayer>> m_ValidationLayers;
};
