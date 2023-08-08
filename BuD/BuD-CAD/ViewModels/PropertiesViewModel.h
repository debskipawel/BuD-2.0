#pragma once

#include <Scene/CompositeSceneObject.h>

struct PropertiesViewModel
{
public:
	PropertiesViewModel(CompositeSceneObject& selectedObjects)
		: m_SelectedObjects(selectedObjects)
	{
	}

	CompositeSceneObject& m_SelectedObjects;
};
