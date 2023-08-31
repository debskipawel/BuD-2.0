#include "bud_pch.h"
#include "RasterizerDescription.h"

bool BuD::RasterizerDescription::operator==(const RasterizerDescription& other) const
{
    return m_CullType == other.m_CullType && m_FillMode == other.m_FillMode;
}

bool BuD::RasterizerDescription::operator!=(const RasterizerDescription& other) const
{
    return !(*this == other);
}

bool BuD::RasterizerDescription::operator>(const RasterizerDescription& other) const
{
    return m_CullType > other.m_CullType 
        || (m_CullType == other.m_CullType && m_FillMode > other.m_FillMode);
}

bool BuD::RasterizerDescription::operator<(const RasterizerDescription& other) const
{
    return m_CullType < other.m_CullType
        || (m_CullType == other.m_CullType && m_FillMode < other.m_FillMode);
}

bool BuD::RasterizerDescription::operator>=(const RasterizerDescription& other) const
{
    return !(*this < other);
}

bool BuD::RasterizerDescription::operator<=(const RasterizerDescription& other) const
{
    return !(*this > other);
}
