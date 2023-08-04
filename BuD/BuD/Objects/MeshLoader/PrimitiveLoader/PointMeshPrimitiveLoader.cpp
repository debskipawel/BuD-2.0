#include "bud_pch.h"
#include "PointMeshPrimitiveLoader.h"

#include <Layout/InputLayoutSystem.h>

#include <SimpleMath.h>
namespace dxm = DirectX::SimpleMath;

namespace BuD::Internal
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> POINT_LAYOUT
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	struct PointVertex
	{
		dxm::Vector4 m_Position;
	};

	static const std::vector<PointVertex> m_PointVertices =
	{
		{ { 0.0f, 0.0f, 0.0f, 1.0f } },
	};

	static const std::vector<unsigned short> m_PointIndices =
	{
		0,
	};

	MeshDetails PointMeshPrimitiveLoader::LoadMesh()
	{
		MeshDetails details;

		details.m_VertexBuffer = std::make_shared<VertexBuffer>(m_PointVertices.size() * sizeof(PointVertex), POINT_LAYOUT, m_PointVertices.data());
		details.m_IndexBuffer = std::make_shared<IndexBuffer>(DXGI_FORMAT_R16_UINT, m_PointIndices.size() * sizeof(unsigned short), m_PointIndices.data(), D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

		details.m_InputLayout = InputLayoutSystem::GetInputLayout(POINT_LAYOUT);
		details.m_Name = "Quad";

		return details;
	}
}

