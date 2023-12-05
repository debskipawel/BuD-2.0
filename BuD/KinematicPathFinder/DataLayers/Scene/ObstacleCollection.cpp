#include "ObstacleCollection.h"

ObstacleCollection::ObstacleCollection(BuD::Scene& scene)
    : m_Scene(scene), m_Obstacles(), m_NewObstacle()
{
}

auto ObstacleCollection::CreateNewObstacle(const dxm::Vector2& v1, const dxm::Vector2& v2) -> std::shared_ptr<Obstacle>
{
    auto rand = BuD::Random();
    auto randomColor = dxm::Vector3(0.25f * rand.Next(), 0.25f + 0.75f * rand.Next(), 0.25f + 0.75f * rand.Next());

    m_NewObstacle = std::make_shared<Obstacle>(m_Scene, v1, v2, randomColor);

    return m_NewObstacle;
}

auto ObstacleCollection::AcceptNewObstacle() -> std::shared_ptr<Obstacle>
{
    if (!m_NewObstacle)
    {
        return {};
    }

    m_Obstacles.push_back(m_NewObstacle);
    m_NewObstacle = {};

    return m_Obstacles.back();
}

auto ObstacleCollection::ModifyNewObstacle(const dxm::Vector2& v2) -> bool
{
    if (!m_NewObstacle)
    {
        return false;
    }

    m_NewObstacle->UpdateSecondVertex(v2);

    return true;
}

auto ObstacleCollection::RemoveObstacle(size_t idx) -> bool
{
    if (idx >= m_Obstacles.size())
    {
        return false;
    }

    m_Obstacles.erase(m_Obstacles.begin() + idx);

    return true;
}

auto ObstacleCollection::GetAll() const -> const std::vector<std::shared_ptr<Obstacle>>&
{
    return m_Obstacles;
}

auto ObstacleCollection::Count() const -> size_t
{
    return m_Obstacles.size();
}
