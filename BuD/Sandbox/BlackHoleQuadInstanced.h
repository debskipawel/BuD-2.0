#pragma once

#include "BlackHoleQuadBase.h"

#include <vector>

class BlackHoleQuadInstanced : public BlackHoleQuadBase
{
public:
	BlackHoleQuadInstanced(BuD::Scene& scene);

protected:
	std::vector<dxm::Matrix> m_InstancesData;
};
