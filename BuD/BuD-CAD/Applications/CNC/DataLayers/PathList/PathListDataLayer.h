#pragma once

#include <string>

#include <Applications/CNC/PathProgram.h>

struct PathListDataLayer
{
	std::vector<std::shared_ptr<PathProgram>> m_PathList;
};
