#pragma once

#include <Scene/SceneCAD.h>

#include <Selection/ManualSelectionSystem.h>
#include <Selection/TransformSelectionSystem.h>

struct SceneDataLayer
{
public:
	SceneCAD m_SceneCAD;

	ManualSelectionSystem m_ManuallySelected;
	TransformSelectionSystem m_SelectedForTransform;
};
