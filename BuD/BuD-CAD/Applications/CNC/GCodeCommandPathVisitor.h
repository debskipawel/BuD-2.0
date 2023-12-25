#pragma once

#include <BuD.h>

#include <Visitors/GCodeCommandVisitor.h>

class GCodeCommandPathVisitor : public GCP::GCodeCommandVisitor
{
public:
	GCodeCommandPathVisitor();

	virtual auto Visit(GCP::GCodeCommand& command) -> void override;

	virtual auto Visit(GCP::FastToolMoveCommand& command) -> void override;
	virtual auto Visit(GCP::SlowToolMoveCommand& command) -> void override;

	virtual auto Visit(GCP::InchesUnitSystemSelectionCommand& command) -> void override;
	virtual auto Visit(GCP::MillimetersUnitSystemSelectionCommand& command) -> void override;

	virtual auto Points() const -> const std::vector<dxm::Vector3>&;

protected:
	float m_UnitScale;

	dxm::Vector3 m_ToolPosition;

	std::vector<dxm::Vector3> m_Points;
};
