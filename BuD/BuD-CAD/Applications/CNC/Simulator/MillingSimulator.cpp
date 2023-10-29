#include "MillingSimulator.h"

MillingSimulator::MillingSimulator()
	: m_Running(false)
{
	ResetToDefault();
}

void MillingSimulator::UploadPath(std::shared_ptr<PathProgram> pathProgram)
{
	if (m_Running)
	{
		return;
	}

	m_UploadedProgram = pathProgram;
}

void MillingSimulator::Start()
{
	if (!m_UploadedProgram)
	{
		return;
	}
	
	ResetToDefault();

	m_Running = true;
}

void MillingSimulator::Stop()
{
	m_Running = false;
	m_Paused = true;
}

void MillingSimulator::Update(float deltaTime)
{
	m_TimeLeft += deltaTime;

	auto& commandList = m_UploadedProgram->m_Program.m_Commands;

	while (m_Running && m_CommandIndex < commandList.size())
	{
		auto& command = commandList[m_CommandIndex];

		GCP::GCodeCommandVisitor::Visit(*command);

		if (m_TimeLeft > 0)
		{
			m_CommandIndex++;
			continue;
		}

		break;
	}

	if (m_CommandIndex >= commandList.size())
	{
		m_Running = false;
	}
}

void MillingSimulator::ResetToDefault()
{
	constexpr auto SLOW_MOVEMENT_SPEED = 25.0f;
	constexpr auto FAST_MOVEMENT_SPEED = 100.0f;
	constexpr auto ROTATION_SPEED = 10000.0f / 60.0f;

	constexpr auto SAFE_MILLING_TOOL_POSITION = dxm::Vector3(0.0f, 30.0f, 0.0f);

	m_UploadedProgram->m_Tool->MoveTo(SAFE_MILLING_TOOL_POSITION);
	m_PreviousToolPosition = m_UploadedProgram->m_Tool->Position();

	SetPositioningAbsolute();
	
	SetToolMovementSpeed(SLOW_MOVEMENT_SPEED, FAST_MOVEMENT_SPEED);
	SetToolRotationSpeed(10000.0f / 60.0f);

	SetUnitSystem(GCP::GCodeUnitSystem::MILLIMETER);

	m_TimeLeft = 0.0f;
}

void MillingSimulator::SetPositioningAbsolute()
{
	m_PositioningAbsolute = true;
}

void MillingSimulator::SetPositioningIncremental()
{
	m_PositioningAbsolute = false;
}

void MillingSimulator::SetToolMovementSpeed(float movementSpeedSlow, float movementSpeedFast)
{
	m_ToolMovementSlowSpeed = movementSpeedSlow;
	m_ToolMovementFastSpeed = movementSpeedFast;
}

void MillingSimulator::SetToolRotationSpeed(float rotationSpeed)
{
	m_ToolRotationSpeed = rotationSpeed;
}

void MillingSimulator::SetUnitSystem(GCP::GCodeUnitSystem unitSystem)
{
	m_UnitSystem = unitSystem;
}

void MillingSimulator::Visit(GCP::FastToolMoveCommand& command)
{
	m_ToolMovementFastSpeed = command.m_MoveSpeed.value_or(m_ToolMovementFastSpeed);

	auto speed = m_ToolMovementFastSpeed;

	auto unitScale = m_CentimeterScaleValuesMap.at(m_UnitSystem);

	auto finalToolPosition = unitScale * dxm::Vector3(
		command.m_X.value_or(m_PreviousToolPosition.x),
		command.m_Y.value_or(m_PreviousToolPosition.y),
		command.m_Z.value_or(m_PreviousToolPosition.z)
	);

	MoveTool(finalToolPosition, speed);
}

void MillingSimulator::Visit(GCP::SlowToolMoveCommand& command)
{
	m_ToolMovementSlowSpeed = command.m_MoveSpeed.value_or(m_ToolMovementSlowSpeed);

	auto speed = m_ToolMovementSlowSpeed;

	auto unitScale = m_CentimeterScaleValuesMap.at(m_UnitSystem);

	auto finalToolPosition = unitScale * dxm::Vector3(
		command.m_X.value_or(m_PreviousToolPosition.x),
		command.m_Y.value_or(m_PreviousToolPosition.y),
		command.m_Z.value_or(m_PreviousToolPosition.z)
	);

	MoveTool(finalToolPosition, speed);
}

void MillingSimulator::Visit(GCP::InchesUnitSystemSelectionCommand& command)
{
	SetUnitSystem(GCP::GCodeUnitSystem::INCHES);
}

void MillingSimulator::Visit(GCP::MillimetersUnitSystemSelectionCommand& command)
{
	SetUnitSystem(GCP::GCodeUnitSystem::MILLIMETER);
}

void MillingSimulator::Visit(GCP::ProgramStopCommand& command)
{
	m_Running = false;
}

void MillingSimulator::Visit(GCP::ToolPositioningAbsoluteCommand& command)
{
	m_PositioningAbsolute = true;
}

void MillingSimulator::Visit(GCP::ToolPositioningIncrementalCommand& command)
{
	m_PositioningAbsolute = false;
}

void MillingSimulator::MoveTool(const dxm::Vector3& finalToolPosition, float speed)
{
	auto toolMoveVector = finalToolPosition - m_PreviousToolPosition;
	toolMoveVector.Normalize();

	auto& tool = m_UploadedProgram->m_Tool;

	auto currentToolPosition = tool->Position();
	auto shiftLeft = finalToolPosition - currentToolPosition;

	auto distanceLeft = shiftLeft.Length();
	auto distance = speed * m_TimeLeft;

	auto timeFractionUsed = std::clamp(distanceLeft / distance, 0.0f, 1.0f);
	m_TimeLeft = (1.0f - timeFractionUsed) * m_TimeLeft;

	auto positionIncrement = m_TimeLeft > 0
		? finalToolPosition - currentToolPosition
		: timeFractionUsed * distance * toolMoveVector;

	tool->MoveBy(positionIncrement);

	if (m_TimeLeft > 0.0f)
	{
		m_PreviousToolPosition = finalToolPosition;
	}
}

std::unordered_map<GCP::GCodeUnitSystem, float> MillingSimulator::m_CentimeterScaleValuesMap = {
	{ GCP::GCodeUnitSystem::MILLIMETER, 0.1f },
	{ GCP::GCodeUnitSystem::INCHES, 2.54f }
};
