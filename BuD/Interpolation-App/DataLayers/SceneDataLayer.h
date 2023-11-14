#pragma once

#include <BuD.h>

#include <Objects/Frame.h>

struct SceneDataLayer
{
	SceneDataLayer();

	BuD::Scene m_EulerScene, m_QuaternionScene;
	Frame m_EulerFrame, m_QuaternionFrame;
};
