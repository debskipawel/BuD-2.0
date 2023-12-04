#pragma once

#include <Scene/Obstacle.h>

class ObstacleCollection
{
public:
	ObstacleCollection(BuD::Scene& scene);

	virtual auto CreateNewObstacle(const dxm::Vector2& v1, const dxm::Vector2& v2) -> std::shared_ptr<Obstacle>;
	virtual auto AcceptNewObstacle() -> std::shared_ptr<Obstacle>;

	virtual auto ModifyNewObstacle(const dxm::Vector2& v2) -> bool;

	virtual auto RemoveObstacle(size_t idx) -> bool;

	virtual auto GetAll() const -> const std::vector<std::shared_ptr<Obstacle>>&;
	virtual auto Count() const -> size_t;

protected:
	BuD::Scene& m_Scene;

	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;
	std::shared_ptr<Obstacle> m_NewObstacle;
};
