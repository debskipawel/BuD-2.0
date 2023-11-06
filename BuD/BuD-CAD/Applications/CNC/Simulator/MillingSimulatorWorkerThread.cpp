#include "MillingSimulatorWorkerThread.h"

#include <Applications/CNC/Objects/Tools/Visitors/ValidationAggregatorFactoryMillingToolVisitor.h>
#include <Applications/CNC/Simulator/MaterialBlockCutter.h>
#include <Applications/CNC/Simulator/MoveValidation/ValidationLayers/AnyContactToolMoveValidationLayer.h>

constexpr auto SLOW_MOVEMENT_SPEED = 25.0f;
constexpr auto FAST_MOVEMENT_SPEED = 100.0f;
constexpr auto ROTATION_SPEED = 10000.0f / 60.0f;

constexpr auto SAFE_MILLING_TOOL_POSITION = dxm::Vector3(0.0f, 30.0f, 0.0f);


MillingSimulatorWorkerThread::MillingSimulatorWorkerThread(MaterialBlockParameters blockParameters, std::shared_ptr<PathProgram> program, std::vector<float>& heightMap)
	: m_MaterialBlockParameters(blockParameters), m_Program(program), m_HeightMap(heightMap), m_CommandIndex(0U), m_TimeLeft(0.0f), m_StoppedOnValidationError(false)
{
	ResetSettingsToDefault();

	m_Program->m_Tool->MoveTo(SAFE_MILLING_TOOL_POSITION);
	m_PreviousToolPosition = m_Program->m_Tool->Position();
}

bool MillingSimulatorWorkerThread::Update(float deltaTime)
{
	auto& commandList = m_Program->m_Program.m_Commands;

	m_TimeLeft += deltaTime;

	while (m_CommandIndex < commandList.size())
	{
		auto& command = commandList[m_CommandIndex];

		GCP::GCodeCommandVisitor::Visit(*command);

		if (m_StoppedOnValidationError)
		{
			return false;
		}

		if (m_TimeLeft > 0.0f)
		{
			m_CommandIndex++;
			continue;
		}

		break;
	}

	return true;
}

void MillingSimulatorWorkerThread::ResetSettingsToDefault()
{
	m_PositioningAbsolute = true;

	m_ToolMovementFastSpeed = FAST_MOVEMENT_SPEED;
	m_ToolMovementSlowSpeed = SLOW_MOVEMENT_SPEED;

	m_ToolRotationSpeed = ROTATION_SPEED;

	m_UnitSystem = GCP::GCodeUnitSystem::MILLIMETER;

	m_TimeLeft = 0.0f;
}

bool MillingSimulatorWorkerThread::Finished() const
{
	const auto& commands = m_Program->m_Program.m_Commands;

	return m_CommandIndex >= commands.size();
}

void MillingSimulatorWorkerThread::Visit(GCP::FastToolMoveCommand& command)
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

void MillingSimulatorWorkerThread::Visit(GCP::SlowToolMoveCommand& command)
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

void MillingSimulatorWorkerThread::MoveTool(dxm::Vector3 finalToolPosition, float speed, MillingValidation validationType)
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

	auto validationAggregator = GetValidationAggregator(validationType);

	auto materialCutter = MaterialBlockCutter(m_MaterialBlockParameters, 
		[this, validationAggregator](int x, int y, ToolCut& toolCut)
		{
			if (this->m_StoppedOnValidationError)
			{
				return;
			}
			
			auto pixelWidth = m_MaterialBlockParameters.m_ResolutionWidth;
			auto pixelHeight = m_MaterialBlockParameters.m_ResolutionHeight;

			auto index = 4 * (y * pixelWidth + x);

			toolCut.m_PreviousHeight = m_HeightMap[index];

			if (std::isnan(toolCut.m_RequestedHeight) || toolCut.m_PreviousHeight <= toolCut.m_RequestedHeight)
			{
				return;
			}

			auto validationErrors = validationAggregator->ValidateMove(toolCut);

			if (validationErrors.size() > 0)
			{
				for (const auto& validationError : validationErrors)
				{
					BuD::Log::WriteError(validationError.m_ErrorMessage);
				}
				
				this->m_StoppedOnValidationError = true;

				return;
			}

			m_HeightMap[index] = toolCut.m_RequestedHeight;
			m_HeightMap[index + 1] = toolCut.m_RequestedHeight;
			m_HeightMap[index + 2] = toolCut.m_RequestedHeight;
			m_HeightMap[index + 3] = toolCut.m_RequestedHeight;
		}
	);

	materialCutter.MoveMillingTool(tool, currentToolPosition + positionIncrement);

	tool->MoveTo(currentToolPosition + positionIncrement);

	if (m_TimeLeft > 0.0f)
	{
		m_PreviousToolPosition = finalToolPosition;
	}
}

std::shared_ptr<ToolMoveValidationAggregator> MillingSimulatorWorkerThread::GetValidationAggregator(MillingValidation validationType)
{
	std::shared_ptr<ToolMoveValidationAggregator> validationAggregator;

	switch (validationType)
	{
		case MillingValidation::NONE:
		{
			std::vector<std::shared_ptr<AbstractToolMoveValidationLayer>> validationLayers;
			
			validationAggregator = std::make_shared<ToolMoveValidationAggregator>(validationLayers);

			break;
		}
		case MillingValidation::EVERY_CONTACT:
		{
			std::vector<std::shared_ptr<AbstractToolMoveValidationLayer>> validationLayers = { std::make_shared<AnyContactToolMoveValidationLayer>(m_MaterialBlockParameters) };

			validationAggregator = std::make_shared<ToolMoveValidationAggregator>(validationLayers);

			break;
		}
		case MillingValidation::TOOL_SPECIFIC:
		{
			auto visitor = std::make_unique<ValidationAggregatorFactoryMillingToolVisitor>(m_MaterialBlockParameters);
			
			const auto& tool = m_Program->m_Tool;

			visitor->Visit(*tool);

			validationAggregator = visitor->CreateValidationAggregator();

			break;
		}
		default:
		{
			break;
		}
	}

	return validationAggregator;
}

void MillingSimulatorWorkerThread::Visit(GCP::InchesUnitSystemSelectionCommand& command)
{
	m_UnitSystem = GCP::GCodeUnitSystem::INCHES;
}

void MillingSimulatorWorkerThread::Visit(GCP::MillimetersUnitSystemSelectionCommand& command)
{
	m_UnitSystem = GCP::GCodeUnitSystem::MILLIMETER;
}

void MillingSimulatorWorkerThread::Visit(GCP::ToolPositioningAbsoluteCommand& command)
{
	m_PositioningAbsolute = true;
}

void MillingSimulatorWorkerThread::Visit(GCP::ToolPositioningIncrementalCommand& command)
{
	m_PositioningAbsolute = false;
}

std::unordered_map<GCP::GCodeUnitSystem, float> MillingSimulatorWorkerThread::s_CentimeterScaleValuesMap = {
	{ GCP::GCodeUnitSystem::MILLIMETER, 0.1f },
	{ GCP::GCodeUnitSystem::INCHES, 2.54f }
};
