#pragma once

#include <Scene/SceneCAD.h>

struct SceneDataLayer
{
public:
	SceneDataLayer(SceneCAD& scene)
		: m_SceneCAD(scene)
	{
	}

	SceneCAD& m_SceneCAD;
};
