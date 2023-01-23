#include "bud_pch.h"
#include "OrbitCamera.h"

using namespace DirectX;

namespace BuD
{
    OrbitCamera::OrbitCamera(dxm::Vector3 target, float minDistance, float maxDistance, float distance)
        : m_AngleX(-0.1f), m_AngleZ(0.0f), m_Target(target.x, target.y, target.z), m_Distance(distance)
    {
        if (maxDistance < minDistance)
        {
            maxDistance = minDistance;
        }

        m_MinDistance = minDistance;
        m_MaxDistance = maxDistance;
    }
    
    OrbitCamera::OrbitCamera(float minDistance, float maxDistance, float distance)
        : OrbitCamera(dxm::Vector3{ 0.0f, 0.0f, 0.0f }, minDistance, maxDistance, distance)
    {
    }
    
    dxm::Matrix BuD::OrbitCamera::ViewMatrix()
    {
        dxm::Vector3 position = EyePosition();

        dxm::Vector3 dir = Front();
        dxm::Vector3 up = dxm::Vector3::Transform(
            dxm::Vector3{ 0.0f, 0.0f, 1.0f },
            dxm::Matrix::CreateRotationX(m_AngleX) * dxm::Matrix::CreateRotationZ(m_AngleZ)
        );

        return dxm::Matrix::CreateLookAt(position, position + dir, up);
    }

    dxm::Vector3 OrbitCamera::EyePosition()
    {
        auto dir = Front();
        return m_Target - m_Distance * dir;
    }

    dxm::Vector3 BuD::OrbitCamera::Front()
    {
        dxm::Vector3 dir = dxm::Vector3::Transform(
            dxm::Vector3{ 0.0f, 1.0f, 0.0f },
            dxm::Matrix::CreateRotationX(m_AngleX) * dxm::Matrix::CreateRotationZ(m_AngleZ)
        );

        return dir;
    }

    dxm::Vector3 BuD::OrbitCamera::Right()
    {
        dxm::Vector3 dir = dxm::Vector3::Transform(
            dxm::Vector3{ 1.0f, 0.0f, 0.0f },
            dxm::Matrix::CreateRotationZ(m_AngleZ)
        );

        return dir;
    }

    dxm::Vector3 BuD::OrbitCamera::Up()
    {
        dxm::Vector3 dir = dxm::Vector3::Transform(
            dxm::Vector3{ 0.0f, 0.0f, 1.0f },
            dxm::Matrix::CreateRotationX(m_AngleX) * dxm::Matrix::CreateRotationZ(m_AngleZ)
        );

        return dir;
    }

    void BuD::OrbitCamera::RotateCamera(float dx, float dy)
    {
        m_AngleX = XMScalarModAngle(m_AngleX - dy);
        m_AngleZ = XMScalarModAngle(m_AngleZ - dx);
    }

    void BuD::OrbitCamera::Zoom(float dd)
    {
        m_Distance += dd;
        ClampDistance();
    }

    void BuD::OrbitCamera::Move(dxm::Vector3 difference)
    {
        m_Target += difference;
    }

    void BuD::OrbitCamera::MoveTo(dxm::Vector3 position)
    {
        m_Target = position;
    }
    
    void OrbitCamera::ClampDistance()
    {
        if (m_Distance < m_MinDistance)
        {
            m_Distance = m_MinDistance;
        }

        if (m_Distance > m_MaxDistance)
        {
            m_Distance = m_MaxDistance;
        }
    }
}
