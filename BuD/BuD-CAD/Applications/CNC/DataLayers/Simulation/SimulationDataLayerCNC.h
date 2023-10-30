#pragma once

#include <unordered_map>

#include <Commands/UnitSystem/GCodeUnitSystem.h>

#include <Applications/CNC/PathProgram.h>
#include <Applications/CNC/Simulator/MillingSimulator.h>

struct SimulationDataLayerCNC
{
public:
	SimulationDataLayerCNC();

	virtual void Update(float deltaTime);
	virtual void ResetMaterial(const MaterialBlockParameters& materialParameters, uint32_t resolutionWidth, uint32_t resolutionHeight);

	virtual void StartSimulation();
	virtual void StopSimulation();

	virtual bool Running() const;
	virtual bool Paused() const;

	auto GetSelectedPath() const -> std::shared_ptr<PathProgram>;
	auto SetSelectedPath(std::shared_ptr<PathProgram> selectedPath) -> void;

	BuD::Scene m_Scene;

	float m_SimulationSpeed;

protected:

	MillingSimulator m_MillingSimulator;

	std::shared_ptr<PathProgram> m_SelectedPath = std::shared_ptr<PathProgram>();
};
