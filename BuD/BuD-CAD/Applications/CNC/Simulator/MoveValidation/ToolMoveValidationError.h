#pragma once

#include <string>

struct ToolMoveValidationError
{
	ToolMoveValidationError(const std::string& errorMessage)
		: m_ErrorMessage(errorMessage)
	{
	}

	std::string m_ErrorMessage;
};
