#include "Plane.h"

Plane::Plane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal)
	: SceneObjectCAD(scene), m_Normal(normal)
{
    m_Transform.m_Position = point;

    if (normal.LengthSquared() < 1e-8f)
    {
        BuD::Log::WriteError("Normal passed to plane constructor was invalid.");
        return;
    }

    auto crossedVectors = std::array<dxm::Vector3, 3> { m_Normal.Cross(dxm::Vector3::UnitX), m_Normal.Cross(dxm::Vector3::UnitY), m_Normal.Cross(dxm::Vector3::UnitZ) };

    m_VectorU = *std::max_element(crossedVectors.begin(), crossedVectors.end(),
        [](const dxm::Vector3& a, const dxm::Vector3& b)
        {
            return a.LengthSquared() < b.LengthSquared();
        }
    );

    m_VectorV = m_Normal.Cross(m_VectorU);

    m_VectorU.Normalize();
    m_VectorV.Normalize();
}

Plane::Plane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& derivativeU, const dxm::Vector3& derivativeV)
    : SceneObjectCAD(scene), m_VectorU(derivativeU), m_VectorV(derivativeV)
{
    m_Transform.m_Position = point;

    if (m_VectorU.LengthSquared() < 1e-6f || m_VectorV.LengthSquared() < 1e-8f)
    {
        BuD::Log::WriteError("Derivative vectors passed to a plane constructor were invalid.");
        return;
    }

    m_VectorU.Normalize();
    m_VectorV.Normalize();

    m_Normal = m_VectorU.Cross(m_VectorV);
}
