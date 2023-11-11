#pragma once

#include <BuD.h>

class Frame
{
public:
	Frame(BuD::Scene& scene);

protected:
	BuD::SceneEntity m_FrameEntity;

	dxm::Matrix m_Model;
};
