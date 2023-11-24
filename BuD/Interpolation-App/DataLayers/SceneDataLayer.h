#pragma once

#include <BuD.h>

#include <Objects/Frame.h>
#include <Objects/GhostFrames.h>
#include <Objects/Grid.h>

struct SceneDataLayer
{
	SceneDataLayer();

	BuD::Scene m_EulerScene, m_QuaternionScene;
	
	Frame m_EulerFrame, m_QuaternionFrame;
	GhostFrames m_EulerGhost, m_QuaternionGhost;

protected:
	Grid m_EulerGrid, m_QuaternionGrid;
};
