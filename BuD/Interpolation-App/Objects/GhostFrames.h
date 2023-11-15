#pragma once

#include <BuD.h>

class GhostFrames
{
public:
	GhostFrames(BuD::Scene& scene);

	virtual void EnableRendering();
	virtual void DisableRendering();

	virtual void UpdateModelMatrices(const std::vector<dxm::Matrix>& matrix);

protected:
	BuD::SceneEntity m_FrameEntity;

	std::vector<dxm::Matrix> m_ModelMatrices;
};
