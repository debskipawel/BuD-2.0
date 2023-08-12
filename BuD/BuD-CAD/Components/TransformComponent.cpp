#include "TransformComponent.h"

bool TransformComponent::operator==(const TransformComponent& other)
{
    return m_Position == other.m_Position && m_Rotation == other.m_Rotation && m_Scale == other.m_Scale;
}

bool TransformComponent::operator!=(const TransformComponent& other)
{
    return !(*this == other);
}

TransformComponent TransformComponent::IDENTITY = {};
