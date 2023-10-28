#include "SimulationDataLayerCNC.h"

SimulationDataLayerCNC::SimulationDataLayerCNC()
	: m_SimulationRunning(false), m_CutterCompensationEnabled(false), m_PositioningAbsolute(true), m_ToolMovementSpeed(25.0f), m_ToolRotationSpeed(10000.0f), m_UnitSystem(GCP::GCodeUnitSystem::MILLIMETER)
{
}

void SimulationDataLayerCNC::ResetToDefault()
{
	m_SimulationRunning = false;
	m_CutterCompensationEnabled = false;
	m_PositioningAbsolute = true;

	m_ToolMovementSpeed = 25.0f;
	m_ToolRotationSpeed = 10000.0f;

	m_UnitSystem = GCP::GCodeUnitSystem::MILLIMETER;
}

std::unordered_map<GCP::GCodeUnitSystem, float> SimulationDataLayerCNC::m_CentimeterScaleValuesMap = {
	{ GCP::GCodeUnitSystem::MILLIMETER, 0.1f },
	{ GCP::GCodeUnitSystem::INCHES, 2.54f }
};
