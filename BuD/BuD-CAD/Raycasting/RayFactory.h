#pragma once

#include "Ray.h"

#include <ViewModels/ViewportViewModel.h>
#include <ViewModels/ObjectListViewModel.h>

class RayFactory
{
public:
	RayFactory(ViewportViewModel& viewport, ObjectListViewModel& objectList);

	Ray CreateRay(dxm::Vector2 screenSpace);

protected:
	ViewportViewModel& m_Viewport;
	ObjectListViewModel& m_ObjectList;
};
