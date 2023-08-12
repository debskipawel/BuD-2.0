#pragma once

#include <Scene/SceneCAD.h>
#include <Scene/SelectedGroup.h>
#include <Actions/ActionList.h>

struct SceneDataLayer
{
public:
	SceneCAD m_SceneCAD;
	SelectedGroup m_SelectedGroup;

	ActionList m_ActionList;
};
