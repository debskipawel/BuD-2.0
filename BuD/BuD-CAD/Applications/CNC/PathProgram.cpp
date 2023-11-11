#include "PathProgram.h"

PathProgram::PathProgram(std::string name, const GCP::GCodeProgram& program, std::shared_ptr<MillingTool> millingTool)
	: m_PathName(name), m_Program(program), m_Tool(millingTool)
{

}
