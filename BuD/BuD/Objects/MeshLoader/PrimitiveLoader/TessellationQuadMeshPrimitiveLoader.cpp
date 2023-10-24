#include "bud_pch.h"
#include "TessellationQuadMeshPrimitiveLoader.h"

#include <Layout/InputLayoutSystem.h>

#include <SimpleMath.h>
namespace dxm = DirectX::SimpleMath;

namespace BuD::Internal
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> TESS_QUAD_LAYOUT
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	struct TessellationQuadVertex
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Normal;
	};

	std::vector<TessellationQuadVertex> m_TessQuadVertices =
	{
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	};

	std::vector<unsigned short> m_TessQuadIndices =
	{
		0, 1, 2, 3
	};

    MeshDetails TessellationQuadMeshPrimitiveLoader::LoadMesh()
    {
		MeshDetails details;

		details.m_VertexBuffer = std::make_shared<VertexBuffer>(m_TessQuadVertices.size() * sizeof(TessellationQuadVertex), TESS_QUAD_LAYOUT, m_TessQuadVertices.data());
		details.m_IndexBuffer = std::make_shared<IndexBuffer>(
			DXGI_FORMAT_R16_UINT, 
			m_TessQuadIndices.size() * sizeof(unsigned short),
			m_TessQuadIndices.data(),
			D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST
		);

		details.m_InputLayout = InputLayoutSystem::GetInputLayout(TESS_QUAD_LAYOUT);
		details.m_Name = "Tessellation quad";

		return details;
    }
}
