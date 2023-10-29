#include "MillingSimulatorCommandVisitor.h"

MillingSimulatorCommandVisitor::MillingSimulatorCommandVisitor(MillingSimulator& millingSimulator)
	: m_MillingSimulator(millingSimulator)
{
}

void MillingSimulatorCommandVisitor::Visit(GCP::FastToolMoveCommand& command)
{
	m_MillingSimulator.m_ToolMovementFastSpeed = command.m_MoveSpeed.value_or(m_MillingSimulator.m_ToolMovementFastSpeed);
	
	auto speed = m_MillingSimulator.m_ToolMovementFastSpeed;

	auto finalToolPosition = dxm::Vector3(
		command.m_X.value_or(m_MillingSimulator.m_PreviousToolPosition.x),
		command.m_X.value_or(m_MillingSimulator.m_PreviousToolPosition.y),
		command.m_X.value_or(m_MillingSimulator.m_PreviousToolPosition.z)
	);

	auto toolMoveVector = finalToolPosition - m_MillingSimulator.m_PreviousToolPosition;
	toolMoveVector.Normalize();

	auto deltaTime = 0.001f;

	auto& tool = m_MillingSimulator.m_UploadedProgram->m_Tool;

	auto currentToolPosition = tool->Position();
	auto shiftLeft = finalToolPosition - currentToolPosition;
	
	auto distanceLeft = shiftLeft.Length();
	auto distance = speed * deltaTime;

	auto timeFractionUsed = std::clamp(distanceLeft / distance, 0.0f, 1.0f);
	auto timeLeft = (1.0f - timeFractionUsed) * deltaTime;

	auto positionIncrement = timeFractionUsed * distance * toolMoveVector;

	tool->MoveBy(positionIncrement);
}

void MillingSimulatorCommandVisitor::Visit(GCP::SlowToolMoveCommand& command)
{
	m_MillingSimulator.m_ToolMovementSlowSpeed = command.m_MoveSpeed.value_or(m_MillingSimulator.m_ToolMovementSlowSpeed);

	dxm::Vector3 finalToolPosition = dxm::Vector3(
		command.m_X.value_or(m_MillingSimulator.m_PreviousToolPosition.x),
		command.m_X.value_or(m_MillingSimulator.m_PreviousToolPosition.y),
		command.m_X.value_or(m_MillingSimulator.m_PreviousToolPosition.z)
	);

	auto toolMoveVector = finalToolPosition - m_MillingSimulator.m_PreviousToolPosition;
	toolMoveVector.Normalize();
}

void MillingSimulatorCommandVisitor::Visit(GCP::InchesUnitSystemSelectionCommand& command)
{
	m_MillingSimulator.SetUnitSystem(GCP::GCodeUnitSystem::INCHES);
}

void MillingSimulatorCommandVisitor::Visit(GCP::MillimetersUnitSystemSelectionCommand& command)
{
	m_MillingSimulator.SetUnitSystem(GCP::GCodeUnitSystem::MILLIMETER);
}

void MillingSimulatorCommandVisitor::Visit(GCP::ProgramStopCommand& command)
{
	m_MillingSimulator.m_Running = false;
}

void MillingSimulatorCommandVisitor::Visit(GCP::ToolPositioningAbsoluteCommand& command)
{
	m_MillingSimulator.m_PositioningAbsolute = true;
}

void MillingSimulatorCommandVisitor::Visit(GCP::ToolPositioningIncrementalCommand& command)
{
	m_MillingSimulator.m_PositioningAbsolute = false;
}
