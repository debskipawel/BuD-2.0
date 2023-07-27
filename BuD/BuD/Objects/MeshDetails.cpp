#include "bud_pch.h"
#include "MeshDetails.h"

namespace BuD
{
	MeshDetails::MeshDetails()
	{
	}
	
	MeshDetails::MeshDetails(const MeshDetails& other)
		: m_Name(other.m_Name), m_VertexBuffer(other.m_VertexBuffer), m_IndexBuffer(other.m_IndexBuffer), m_InputLayout(other.m_InputLayout)
	{
		if (other.m_BoundingBoxCallback.has_value())
		{
			m_BoundingBoxCallback = other.m_BoundingBoxCallback;
		}
	}
	
	bool MeshDetails::operator==(const MeshDetails& other) const
	{
		return m_VertexBuffer == other.m_VertexBuffer && m_IndexBuffer == m_IndexBuffer && m_InputLayout == other.m_InputLayout;
	}

	bool MeshDetails::operator!=(const MeshDetails& other) const
	{
		return !(*this == other);
	}

	bool MeshDetails::operator<=(const MeshDetails& other) const
	{
		return !(*this > other);
	}
	
	bool MeshDetails::operator>=(const MeshDetails& other) const
	{
		return !(*this < other);
	}
	
	bool MeshDetails::operator<(const MeshDetails& other) const
	{
		return m_VertexBuffer < other.m_VertexBuffer
			|| (m_VertexBuffer == other.m_VertexBuffer && m_IndexBuffer < other.m_IndexBuffer)
			|| (m_VertexBuffer == other.m_VertexBuffer && m_IndexBuffer == other.m_IndexBuffer && m_InputLayout < other.m_InputLayout);
	}
	
	bool MeshDetails::operator>(const MeshDetails& other) const
	{
		return m_VertexBuffer > other.m_VertexBuffer
			|| (m_VertexBuffer == other.m_VertexBuffer && m_IndexBuffer > other.m_IndexBuffer)
			|| (m_VertexBuffer == other.m_VertexBuffer && m_IndexBuffer == other.m_IndexBuffer && m_InputLayout > other.m_InputLayout);;
	}
}
