#pragma once

#include <Applications/CNC/Objects/Tools/Visitors/MillingToolVisitor.h>
#include <Applications/CNC/Simulator/MoveValidation/ToolMoveValidationAggregator.h>

class ValidationAggregatorFactoryMillingToolVisitor : public MillingToolVisitor
{
public:
	ValidationAggregatorFactoryMillingToolVisitor(MaterialBlockParameters materialParameters);
	
	virtual std::shared_ptr<ToolMoveValidationAggregator> CreateValidationAggregator();

	virtual void Visit(MillingTool& millingTool) override;

	virtual void Visit(FlatMillingTool& millingTool) override;
	virtual void Visit(SphericalMillingTool& millingTool) override;

protected:
	MaterialBlockParameters m_MaterialParameters;

	std::vector<std::shared_ptr<AbstractToolMoveValidationLayer>> m_ValidationLayers;
};
