#include "MillingSimulatorThread.h"

#include <Applications/CNC/Simulator/MaterialBlockCutter.h>

constexpr auto SLOW_MOVEMENT_SPEED = 25.0f;
constexpr auto FAST_MOVEMENT_SPEED = 100.0f;
constexpr auto ROTATION_SPEED = 10000.0f / 60.0f;

constexpr auto SAFE_MILLING_TOOL_POSITION = dxm::Vector3(0.0f, 30.0f, 0.0f);


MillingSimulatorThread::MillingSimulatorThread(MaterialBlockParameters blockParameters, std::shared_ptr<PathProgram> program, std::vector<float>& heightMap)
	: m_MaterialBlockParameters(blockParameters), m_Program(program), m_HeightMap(heightMap), m_CommandIndex(0U), m_TimeLeft(0.0f)
{
	ResetSettingsToDefault();

	m_Program->m_Tool->MoveTo(SAFE_MILLING_TOOL_POSITION);
	m_PreviousToolPosition = m_Program->m_Tool->Position();
}

void MillingSimulatorThread::Update(float deltaTime)
{
	auto& commandList = m_Program->m_Program.m_Commands;

	m_TimeLeft += deltaTime;

	while (m_CommandIndex < commandList.size())
	{
		auto& command = commandList[m_CommandIndex];

		GCP::GCodeCommandVisitor::Visit(*command);

		if (m_TimeLeft > 0.0f)
		{
			m_CommandIndex++;
			continue;
		}

		break;
	}
}

void MillingSimulatorThread::ResetSettingsToDefault()
{
	m_PositioningAbsolute = true;

	m_ToolMovementFastSpeed = FAST_MOVEMENT_SPEED;
	m_ToolMovementSlowSpeed = SLOW_MOVEMENT_SPEED;

	m_ToolRotationSpeed = ROTATION_SPEED;

	m_UnitSystem = GCP::GCodeUnitSystem::MILLIMETER;

	m_TimeLeft = 0.0f;
}

bool MillingSimulatorThread::Finished() const
{
	const auto& commands = m_Program->m_Program.m_Commands;

	return m_CommandIndex >= commands.size();
}

void MillingSimulatorThread::Visit(GCP::FastToolMoveCommand& command)
{
	m_ToolMovementSlowSpeed = command.m_MoveSpeed.value_or(m_ToolMovementSlowSpeed);

	auto speed = m_ToolMovementSlowSpeed;

	auto unitScale = s_CentimeterScaleValuesMap.at(m_UnitSystem);

	auto finalToolPosition = unitScale * dxm::Vector3(
		command.m_X.value_or(m_PositioningAbsolute ? m_PreviousToolPosition.x : 0.0f),
		command.m_Y.value_or(m_PositioningAbsolute ? m_PreviousToolPosition.y : 0.0f),
		command.m_Z.value_or(m_PositioningAbsolute ? m_PreviousToolPosition.z : 0.0f)
	);

	if (!m_PositioningAbsolute)
	{
		finalToolPosition += m_PreviousToolPosition;
	}

	MoveTool(finalToolPosition, speed, MillingValidation::EVERY_CONTACT);
}

void MillingSimulatorThread::Visit(GCP::SlowToolMoveCommand& command)
{
	m_ToolMovementSlowSpeed = command.m_MoveSpeed.value_or(m_ToolMovementSlowSpeed);

	auto speed = m_ToolMovementSlowSpeed;

	auto unitScale = s_CentimeterScaleValuesMap.at(m_UnitSystem);

	auto finalToolPosition = unitScale * dxm::Vector3(
		command.m_X.value_or(m_PositioningAbsolute ? m_PreviousToolPosition.x : 0.0f),
		command.m_Y.value_or(m_PositioningAbsolute ? m_PreviousToolPosition.y : 0.0f),
		command.m_Z.value_or(m_PositioningAbsolute ? m_PreviousToolPosition.z : 0.0f)
	);

	if (!m_PositioningAbsolute)
	{
		finalToolPosition += m_PreviousToolPosition;
	}

	MoveTool(finalToolPosition, speed, MillingValidation::TOOL_SPECIFIC);
}

void MillingSimulatorThread::Visit(GCP::InchesUnitSystemSelectionCommand& command)
{
	m_UnitSystem = GCP::GCodeUnitSystem::INCHES;
}

void MillingSimulatorThread::Visit(GCP::MillimetersUnitSystemSelectionCommand& command)
{
	m_UnitSystem = GCP::GCodeUnitSystem::MILLIMETER;
}

void MillingSimulatorThread::Visit(GCP::ProgramStopCommand& command)
{
}

void MillingSimulatorThread::Visit(GCP::ToolPositioningAbsoluteCommand& command)
{
	m_PositioningAbsolute = true;
}

void MillingSimulatorThread::Visit(GCP::ToolPositioningIncrementalCommand& command)
{
	m_PositioningAbsolute = false;
}

void MillingSimulatorThread::MoveTool(dxm::Vector3 finalToolPosition, float speed, MillingValidation validationType)
{
	auto toolMoveVector = finalToolPosition - m_PreviousToolPosition;
	toolMoveVector.Normalize();

	auto& tool = m_Program->m_Tool;

	auto currentToolPosition = tool->Position();
	auto shiftLeft = finalToolPosition - currentToolPosition;

	auto distanceLeft = shiftLeft.Length();
	auto distance = speed * m_TimeLeft;

	auto timeFractionUsed = std::clamp(distanceLeft / distance, 0.0f, 1.0f);
	m_TimeLeft = (1.0f - timeFractionUsed) * m_TimeLeft;

	auto positionIncrement = m_TimeLeft > 0.0f
		? finalToolPosition - currentToolPosition
		: timeFractionUsed * distance * toolMoveVector;

	auto materialCutter = MaterialBlockCutter(m_MaterialBlockParameters, 
		[this](int x, int y, float height) 
		{
			auto pixelWidth = m_MaterialBlockParameters.m_ResolutionWidth;
			auto pixelHeight = m_MaterialBlockParameters.m_ResolutionHeight;

			auto index = 4 * (y * pixelWidth + x);

			auto currentHeight = m_HeightMap[index];

			if (currentHeight <= height || std::isnan(height))
			{
				return;
			}

			m_HeightMap[index] = height;
			m_HeightMap[index + 1] = height;
			m_HeightMap[index + 2] = height;
			m_HeightMap[index + 3] = height;
		}
	);

	materialCutter.MoveMillingTool(tool, currentToolPosition + positionIncrement);

	tool->MoveTo(currentToolPosition + positionIncrement);

	if (m_TimeLeft > 0.0f)
	{
		m_PreviousToolPosition = finalToolPosition;
	}
}

std::unordered_map<GCP::GCodeUnitSystem, float> MillingSimulatorThread::s_CentimeterScaleValuesMap = {
	{ GCP::GCodeUnitSystem::MILLIMETER, 0.1f },
	{ GCP::GCodeUnitSystem::INCHES, 2.54f }
};
