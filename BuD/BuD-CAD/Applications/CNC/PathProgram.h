#pragma once

#include <GCodeProgram.h>

#include <Applications/CNC/Objects/Tools/MillingTool.h>

struct PathProgram
{
public:
	PathProgram(BuD::Scene& scene, std::string name, const GCP::GCodeProgram& program, std::shared_ptr<MillingTool> millingTool);

	virtual auto DisableRendering() -> void;
	virtual auto EnableRendering() -> void;

	std::string m_PathName;
	GCP::GCodeProgram m_Program;

	std::shared_ptr<MillingTool> m_Tool;

protected:
	BuD::SceneEntity m_PathEntity;
};

