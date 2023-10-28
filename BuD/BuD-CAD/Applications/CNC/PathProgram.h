#pragma once

#include <GCodeProgram.h>

#include <Applications/CNC/Objects/Tools/MillingTool.h>

struct PathProgram
{
public:
	PathProgram(std::string name, const GCP::GCodeProgram& program, std::shared_ptr<MillingTool> millingTool);

	std::string m_PathName;
	GCP::GCodeProgram m_Program;

	std::shared_ptr<MillingTool> m_Tool;
};

