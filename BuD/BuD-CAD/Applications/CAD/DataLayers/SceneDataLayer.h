#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Selection/ManualSelectionSystem.h>
#include <Applications/CAD/Selection/TransformSelectionSystem.h>

struct SceneDataLayer
{
public:
	SceneCAD m_SceneCAD;

	std::filesystem::path m_PathToFile = std::filesystem::path();

	ManualSelectionSystem m_ManuallySelected;
	TransformSelectionSystem m_SelectedForTransform;
};
