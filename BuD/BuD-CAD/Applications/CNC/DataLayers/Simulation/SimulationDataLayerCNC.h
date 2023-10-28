#pragma once

#include <unordered_map>

#include <Commands/UnitSystem/GCodeUnitSystem.h>

#include <Applications/CNC/PathProgram.h>

struct SimulationDataLayerCNC
{
public:
	SimulationDataLayerCNC();

	virtual void ResetToDefault();

	bool m_SimulationRunning;

	bool m_PositioningAbsolute;
	bool m_CutterCompensationEnabled;

	float m_ToolMovementSpeed;
	float m_ToolRotationSpeed;

	GCP::GCodeUnitSystem m_UnitSystem;

	std::shared_ptr<PathProgram> m_SelectedPath = std::shared_ptr<PathProgram>();

private:
	static std::unordered_map<GCP::GCodeUnitSystem, float> m_CentimeterScaleValuesMap;
};
