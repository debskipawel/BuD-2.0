#pragma once

#include <optional>

#include <Applications/CNC/Simulator/ToolCut.h>
#include <Applications/CNC/Simulator/MoveValidation/ToolMoveValidationError.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

class AbstractToolMoveValidationLayer
{
public:
	AbstractToolMoveValidationLayer(MaterialBlockParameters materialParameters);

	virtual std::optional<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut) = 0;

protected:
	MaterialBlockParameters m_MaterialParameters;
};
