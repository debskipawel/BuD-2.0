#pragma once

#include <Scene/Obstacle.h>

class CollisionDetector
{
public:
	virtual auto IsColliding(const Obstacle& obstacle, const dxm::Vector2& p0, const dxm::Vector2& p1) -> bool;
};
