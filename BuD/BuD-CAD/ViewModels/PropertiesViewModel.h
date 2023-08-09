#pragma once

#include <Scene/SceneCAD.h>

struct PropertiesViewModel
{
public:
	PropertiesViewModel(SceneCAD& scene)
		: m_SceneCAD(scene)
	{
	}

	SceneCAD& m_SceneCAD;
};
