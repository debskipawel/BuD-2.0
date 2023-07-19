#include "bud_pch.h"
#include "QuadMeshPrimitiveLoader.h"

#include <Layout/InputLayoutSystem.h>

#include <SimpleMath.h>
namespace dxm = DirectX::SimpleMath;

namespace BuD::Internal
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> QUAD_LAYOUT
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	struct CubeVertex
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Normal;
	};

	std::vector<CubeVertex> m_QuadVertices =
	{
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
	};

	std::vector<unsigned short> m_QuadIndices =
	{
		0, 3, 2,
		0, 2, 1
	};

	MeshDetails QuadMeshPrimitiveLoader::LoadMesh()
    {
		MeshDetails details;

		details.m_VertexBuffer = std::make_shared<VertexBuffer>(m_QuadVertices.size() * sizeof(CubeVertex), QUAD_LAYOUT, m_QuadVertices.data());
		details.m_IndexBuffer = std::make_shared<IndexBuffer>(DXGI_FORMAT_R16_UINT, m_QuadIndices.size() * sizeof(unsigned short), m_QuadIndices.data());

		details.m_InputLayout = InputLayoutSystem::GetInputLayout(QUAD_LAYOUT);
		details.m_Name = "Quad";

		return details;
    }
}

