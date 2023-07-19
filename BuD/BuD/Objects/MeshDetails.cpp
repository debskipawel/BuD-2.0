#include "bud_pch.h"
#include "MeshDetails.h"

namespace BuD
{
	bool MeshDetails::operator==(const MeshDetails& other) const
	{
		return m_VertexBuffer == other.m_VertexBuffer && m_IndexBuffer == m_IndexBuffer && m_InputLayout == other.m_InputLayout;
	}

	bool MeshDetails::operator!=(const MeshDetails& other) const
	{
		return !(*this == other);
	}
}
