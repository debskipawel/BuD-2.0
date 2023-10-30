#pragma once

#include <mutex>
#include <thread>

#include <Commands/UnitSystem/GCodeUnitSystem.h>
#include <Visitors/GCodeCommandVisitor.h>

#include <Applications/CNC/PathProgram.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlock.h>

class MillingSimulator : public GCP::GCodeCommandVisitor
{
public:
	MillingSimulator(BuD::Scene& scene);

	virtual void UploadPath(std::shared_ptr<PathProgram> pathProgram);
	virtual void ResetMaterial(const MaterialBlockParameters& materialParameters, uint32_t resolutionWidth, uint32_t resolutionHeight);

	virtual void Start();
	virtual void Stop();

	virtual bool Running() const { return m_Running; }
	virtual bool Paused() const { return m_Paused; }

	virtual void Update(float deltaTime);
	virtual void ResetToDefault();

	virtual void SimulationLoop();

	virtual void Visit(GCP::FastToolMoveCommand& command) override;
	virtual void Visit(GCP::SlowToolMoveCommand& command) override;

	virtual void Visit(GCP::InchesUnitSystemSelectionCommand& command) override;
	virtual void Visit(GCP::MillimetersUnitSystemSelectionCommand& command) override;

	virtual void Visit(GCP::ProgramStopCommand& command) override;

	virtual void Visit(GCP::ToolPositioningAbsoluteCommand& command) override;
	virtual void Visit(GCP::ToolPositioningIncrementalCommand& command) override;

protected:
	virtual void MoveTool(const dxm::Vector3& finalToolPosition, float speed);

	std::jthread m_SimulationThread;

	bool m_Running;
	bool m_Paused;

	bool m_PositioningAbsolute;

	float m_ToolMovementSlowSpeed;
	float m_ToolMovementFastSpeed;
	float m_ToolRotationSpeed;

	GCP::GCodeUnitSystem m_UnitSystem;

	float m_TimeLeft;
	std::mutex m_TimeLeftMutex;

	size_t m_CommandIndex;

	dxm::Vector3 m_PreviousToolPosition;

	std::shared_ptr<PathProgram> m_UploadedProgram;
	std::unique_ptr<MaterialBlock> m_MaterialBlock;

private:
	static std::unordered_map<GCP::GCodeUnitSystem, float> s_CentimeterScaleValuesMap;
};
