#include "ValidationAggregatorFactoryMillingToolVisitor.h"

#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/NonMillingPartToolMoveValidationLayer.h>
#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/ToolImmersionToolMoveValidationLayer.h>
#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/VerticalMovementToolMoveValidationLayer.h>

ValidationAggregatorFactoryMillingToolVisitor::ValidationAggregatorFactoryMillingToolVisitor(MaterialBlockParameters materialParameters)
	: m_ValidationLayers(), m_MaterialParameters(materialParameters)
{
}

std::shared_ptr<ToolMoveValidationAggregator> ValidationAggregatorFactoryMillingToolVisitor::CreateValidationAggregator()
{
	return std::make_shared<ToolMoveValidationAggregator>(m_ValidationLayers);
}

void ValidationAggregatorFactoryMillingToolVisitor::Visit(MillingTool& millingTool)
{
	m_ValidationLayers.clear();

	MillingToolVisitor::Visit(millingTool);
}

void ValidationAggregatorFactoryMillingToolVisitor::Visit(FlatMillingTool& millingTool)
{
	m_ValidationLayers.emplace_back(std::make_shared<NonMillingPartToolMoveValidationLayer>(m_MaterialParameters));
	m_ValidationLayers.emplace_back(std::make_shared<ToolImmersionToolMoveValidationLayer>(m_MaterialParameters));
	m_ValidationLayers.emplace_back(std::make_shared<VerticalMovementToolMoveValidationLayer>(m_MaterialParameters));
}

void ValidationAggregatorFactoryMillingToolVisitor::Visit(SphericalMillingTool& millingTool)
{
	m_ValidationLayers.emplace_back(std::make_shared<NonMillingPartToolMoveValidationLayer>(m_MaterialParameters));
	m_ValidationLayers.emplace_back(std::make_shared<ToolImmersionToolMoveValidationLayer>(m_MaterialParameters));
}
