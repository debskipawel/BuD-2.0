#include "ToolMoveValidationAggregator.h"

ToolMoveValidationAggregator::ToolMoveValidationAggregator(const std::vector<std::shared_ptr<AbstractToolMoveValidationLayer>>& validationLayers)
    : m_ValidationLayers(validationLayers)
{
}

std::vector<ToolMoveValidationError> ToolMoveValidationAggregator::ValidateMove(const ToolCut& toolCut)
{
    auto errors = std::vector<ToolMoveValidationError>();

    for (const auto& validationLayer : m_ValidationLayers)
    {
        auto validationResult = validationLayer->ValidateMove(toolCut);

        if (validationResult.has_value())
        {
            errors.push_back(validationResult.value());
        }
    }

    return errors;
}
