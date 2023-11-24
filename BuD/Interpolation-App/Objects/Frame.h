#pragma once

#include <BuD.h>

class Frame
{
public:
	Frame(BuD::Scene& scene);

	virtual void SetModelMatrix(const dxm::Matrix& matrix);

protected:
	BuD::SceneEntity m_FrameEntity;

	dxm::Matrix m_Model;
};
