#include "Frame.h"

Frame::Frame(const dxm::Vector3& position, const dxm::Vector3& front, const dxm::Vector3& up)
    : m_Position(position), m_Front(front), m_Up(up)
{
    m_Front.Normalize();
    m_Up.Normalize();
}

auto Frame::Translate(const dxm::Vector3& translation) const -> Frame
{
    return Frame(m_Position + translation, m_Front, m_Up);
}

auto Frame::Rotate(const dxm::Matrix& rotationMatrix) const -> Frame
{
    auto rotatedFront = dxm::Vector3::Transform(m_Front, rotationMatrix);
    auto rotatedUp = dxm::Vector3::Transform(m_Up, rotationMatrix);

    return Frame(m_Position, rotatedFront, rotatedUp);
}

auto Frame::Rotate(const dxm::Quaternion& rotation) const -> Frame
{
    auto rotatedFront = dxm::Vector3::Transform(m_Front, rotation);
    auto rotatedUp = dxm::Vector3::Transform(m_Up, rotation);

    return Frame(m_Position, rotatedFront, rotatedUp);
}

auto Frame::Front() const -> dxm::Vector3
{
    return m_Front;
}

auto Frame::Up() const -> dxm::Vector3
{
    return m_Up;
}

auto Frame::Right() const -> dxm::Vector3
{
    auto crossed = m_Front.Cross(m_Up);
    crossed.Normalize();

    return crossed;
}

auto Frame::Position() const -> dxm::Vector3
{
    return m_Position;
}
