#pragma once

#include <Visitors/GCodeCommandVisitor.h>

#include <Applications/CNC/Simulator/MillingSimulator.h>

class MillingSimulatorCommandVisitor : public GCP::GCodeCommandVisitor
{
public:
	MillingSimulatorCommandVisitor(MillingSimulator& millingSimulator);

	virtual void Visit(GCP::FastToolMoveCommand& command) override;
	virtual void Visit(GCP::SlowToolMoveCommand& command) override;

	virtual void Visit(GCP::InchesUnitSystemSelectionCommand& command) override;
	virtual void Visit(GCP::MillimetersUnitSystemSelectionCommand& command) override;

	virtual void Visit(GCP::ProgramStopCommand& command) override;

	virtual void Visit(GCP::ToolPositioningAbsoluteCommand& command) override;
	virtual void Visit(GCP::ToolPositioningIncrementalCommand& command) override;

protected:
	MillingSimulator& m_MillingSimulator;
};
