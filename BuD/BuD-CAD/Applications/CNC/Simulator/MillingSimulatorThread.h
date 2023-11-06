#pragma once

#include <vector>

#include <Visitors/GCodeCommandVisitor.h>

#include <Applications/CNC/PathProgram.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

#include <Applications/CNC/Simulator/MillingValidation.h>
#include <Applications/CNC/Simulator/MoveValidation/ToolMoveValidationAggregator.h>

class MillingSimulatorThread : public GCP::GCodeCommandVisitor
{
public:
	MillingSimulatorThread(MaterialBlockParameters blockParameters, std::shared_ptr<PathProgram> program, std::vector<float>& heightMap);

	virtual bool Update(float deltaTime);

	virtual void ResetSettingsToDefault();
	virtual bool Finished() const;

	virtual void Visit(GCP::FastToolMoveCommand& command) override;
	virtual void Visit(GCP::SlowToolMoveCommand& command) override;

	virtual void Visit(GCP::InchesUnitSystemSelectionCommand& command) override;
	virtual void Visit(GCP::MillimetersUnitSystemSelectionCommand& command) override;

	virtual void Visit(GCP::ToolPositioningAbsoluteCommand& command) override;
	virtual void Visit(GCP::ToolPositioningIncrementalCommand& command) override;

	bool m_StoppedOnValidationError;

protected:
	virtual void MoveTool(dxm::Vector3 finalToolPosition, float speed, MillingValidation validationType);

	virtual std::shared_ptr<ToolMoveValidationAggregator> GetValidationAggregator(MillingValidation validationType);

	MaterialBlockParameters m_MaterialBlockParameters;
	std::shared_ptr<PathProgram> m_Program;
	std::vector<float>& m_HeightMap;

	uint32_t m_CommandIndex;
	float m_TimeLeft;

	// ---------- MACHINE PARAMETERS ------------
	bool m_PositioningAbsolute;

	float m_ToolMovementSlowSpeed;
	float m_ToolMovementFastSpeed;
	float m_ToolRotationSpeed;

	GCP::GCodeUnitSystem m_UnitSystem;

	dxm::Vector3 m_PreviousToolPosition;

private:
	static std::unordered_map<GCP::GCodeUnitSystem, float> s_CentimeterScaleValuesMap;
};
