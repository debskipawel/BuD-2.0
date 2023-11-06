#pragma once

#include <unordered_map>

#include <Commands/UnitSystem/GCodeUnitSystem.h>

#include <Applications/CNC/PathProgram.h>
#include <Applications/CNC/Simulator/MillingSimulator.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlock.h>

struct SimulationDataLayerCNC
{
public:
	SimulationDataLayerCNC();

	virtual void Update(float deltaTime);
	virtual void ResetMaterial(const MaterialBlockParameters& materialParameters);

	virtual void StartSimulation();
	virtual void StopSimulation();
	virtual void SkipSimulation();

	virtual bool Running() const;

	auto GetSelectedPath() const -> std::shared_ptr<PathProgram>;
	auto SetSelectedPath(std::shared_ptr<PathProgram> selectedPath) -> void;

	BuD::Scene m_Scene;

	float m_SimulationSpeed;

protected:

	MillingSimulator m_MillingSimulator;

	MaterialBlock m_MaterialBlockMesh;

	std::shared_ptr<PathProgram> m_SelectedPath = std::shared_ptr<PathProgram>();
};
