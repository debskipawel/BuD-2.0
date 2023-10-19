#pragma once

#include "Ray.h"

#include <BuD.h>

class RayFactory
{
public:
	RayFactory(std::shared_ptr<BuD::AbstractCamera> camera);

	Ray CreateRay(dxm::Vector2 screenSpace);

protected:
	std::shared_ptr<BuD::AbstractCamera> m_Camera;
};
