#include "bud_pch.h"
#include "RenderingPass.h"

namespace BuD
{
    bool RenderingPass::operator==(const RenderingPass& other) const
    {
        return m_Mesh == other.m_Mesh 
            && m_Pipeline == other.m_Pipeline;
    }

    bool RenderingPass::operator!=(const RenderingPass& other) const
    {
        return !(*this == other);
    }

    bool RenderingPass::operator<=(const RenderingPass& other) const
    {
        return !(*this > other);
    }
    
    bool RenderingPass::operator>=(const RenderingPass& other) const
    {
        return !(*this < other);
    }
    
    bool RenderingPass::operator<(const RenderingPass& other) const
    {
        return m_Mesh < other.m_Mesh
            || (m_Mesh == other.m_Mesh && m_Pipeline < other.m_Pipeline)
            || (m_Mesh == other.m_Mesh && m_Pipeline == other.m_Pipeline && m_RasterizerDescription < other.m_RasterizerDescription);
    }
    
    bool RenderingPass::operator>(const RenderingPass& other) const
    {
        return m_Mesh > other.m_Mesh
            || (m_Mesh == other.m_Mesh && m_Pipeline > other.m_Pipeline)
            || (m_Mesh == other.m_Mesh && m_Pipeline == other.m_Pipeline && m_RasterizerDescription > other.m_RasterizerDescription);
    }
}
