#pragma once

#include <vector>

#include <Scene/SceneCAD.h>

struct ObjectListViewModel
{
public:
	ObjectListViewModel(SceneCAD& scene)
		: m_Scene(scene)
	{
	}

	SceneCAD& m_Scene;
};
