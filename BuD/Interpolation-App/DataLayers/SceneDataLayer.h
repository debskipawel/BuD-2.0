#pragma once

#include <BuD.h>

#include <Objects/Frame.h>
#include <Objects/Grid.h>

struct SceneDataLayer
{
	SceneDataLayer();

	BuD::Scene m_EulerScene, m_QuaternionScene;
	Frame m_EulerFrame, m_QuaternionFrame;

protected:
	Grid m_EulerGrid, m_QuaternionGrid;
};
