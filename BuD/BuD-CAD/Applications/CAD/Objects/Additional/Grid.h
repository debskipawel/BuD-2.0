#pragma once

#include <BuD.h>

class Grid
{
public:
	explicit Grid(BuD::Scene& scene);

protected:
	BuD::SceneEntity m_GridEntity;
};
