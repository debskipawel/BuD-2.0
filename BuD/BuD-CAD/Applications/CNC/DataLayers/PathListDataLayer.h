#pragma once

#include <string>

#include <GCodeProgram.h>

struct ToolPath
{
	ToolPath(std::string name, GCP::GCodeProgram program)
		: m_PathName(name), m_Program(program)
	{
	}

	std::string m_PathName;
	GCP::GCodeProgram m_Program;
};

struct PathListDataLayer
{
	std::vector<std::shared_ptr<ToolPath>> m_PathList;

	std::shared_ptr<ToolPath> m_SelectedPath = std::shared_ptr<ToolPath>();
};
