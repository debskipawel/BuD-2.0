#include "InfinitePlane.h"

#include <Applications/CAD/Visitors/AbstractVisitor.h>

InfinitePlane::InfinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal)
    : SceneObjectCAD(scene), m_PointOnPlane(point), m_Normal(normal)
{
    m_Tag = "Infinite plane";

    if (normal.LengthSquared() < 1e-6f)
    {
        BuD::Log::WriteError("Normal passed to infinite plane constructor was invalid.");
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

auto InfinitePlane::Accept(AbstractVisitor& visitor) -> void
{
    visitor.Visit(*this);
}

auto InfinitePlane::GetPoint(float u, float v) const -> dxm::Vector3
{
    return m_PointOnPlane + u * m_VectorU + v * m_VectorV;
}
