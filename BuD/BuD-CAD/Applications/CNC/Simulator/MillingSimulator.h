#pragma once

#include <Commands/UnitSystem/GCodeUnitSystem.h>
#include <Visitors/GCodeCommandVisitor.h>

#include <Applications/CNC/PathProgram.h>

class MillingSimulator : public GCP::GCodeCommandVisitor
{
public:
	MillingSimulator();

	virtual void UploadPath(std::shared_ptr<PathProgram> pathProgram);
	
	virtual void Start();
	virtual void Stop();

	virtual bool Running() const { return m_Running; }
	virtual bool Paused() const { return m_Paused; }

	virtual void Update(float deltaTime);
	virtual void ResetToDefault();

	virtual void SetPositioningAbsolute();
	virtual void SetPositioningIncremental();

	virtual void SetToolMovementSpeed(float movementSpeedSlow, float movementSpeedFast);
	virtual void SetToolRotationSpeed(float rotationSpeed);

	virtual void SetUnitSystem(GCP::GCodeUnitSystem unitSystem);

	virtual void Visit(GCP::FastToolMoveCommand& command) override;
	virtual void Visit(GCP::SlowToolMoveCommand& command) override;

	virtual void Visit(GCP::InchesUnitSystemSelectionCommand& command) override;
	virtual void Visit(GCP::MillimetersUnitSystemSelectionCommand& command) override;

	virtual void Visit(GCP::ProgramStopCommand& command) override;

	virtual void Visit(GCP::ToolPositioningAbsoluteCommand& command) override;
	virtual void Visit(GCP::ToolPositioningIncrementalCommand& command) override;

protected:
	bool m_Running;
	bool m_Paused;

	bool m_PositioningAbsolute;

	float m_ToolMovementSlowSpeed;
	float m_ToolMovementFastSpeed;
	float m_ToolRotationSpeed;

	float m_TimeLeft;
	size_t m_CommandIndex;

	GCP::GCodeUnitSystem m_UnitSystem;

	std::shared_ptr<PathProgram> m_UploadedProgram;
	dxm::Vector3 m_PreviousToolPosition;

private:
	static std::unordered_map<GCP::GCodeUnitSystem, float> m_CentimeterScaleValuesMap;

	friend class MillingSimulatorCommandVisitor;
};
