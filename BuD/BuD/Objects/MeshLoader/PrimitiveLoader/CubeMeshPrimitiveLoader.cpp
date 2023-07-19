#include "bud_pch.h"
#include "CubeMeshPrimitiveLoader.h"

#include <Layout/InputLayoutSystem.h>

#include <SimpleMath.h>
namespace dxm = DirectX::SimpleMath;

namespace BuD::Internal
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> CUBE_LAYOUT
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	struct CubeVertex
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Normal;
	};

	std::vector<CubeVertex> m_CubeVertices =
	{
		// FRONT
		{ { -1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },
		{ {  1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },

		// BACK
		{ { -1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },

		// LEFT
		{ { -1.0f, -1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f } },

		// RIGHT
		{ {  1.0f, -1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f } },
		{ {  1.0f,  1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f } },

		// UP
		{ { -1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f } },
		{ {  1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f } },

		// DOWN
		{ { -1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f } },
		{ {  1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f } },
	};

	std::vector<unsigned short> m_CubeIndices =
	{
		0,  3,  2,  0,  2,  1,		// FRONT
		4,  6,  7,  4,  5,  6,		// BACK
		8,  11, 9,  9,  11, 10,		// LEFT
		12, 13, 15, 13, 14, 15,		// RIGHT
		16, 18, 17, 16, 19, 18,		// UP
		20, 21, 22, 20, 22, 23,		// DOWN
	};

	MeshDetails CubeMeshPrimitiveLoader::LoadMesh()
	{
		MeshDetails details;

		details.m_VertexBuffer = std::make_shared<VertexBuffer>(m_CubeVertices.size() * sizeof(CubeVertex), CUBE_LAYOUT, m_CubeVertices.data());
		details.m_IndexBuffer = std::make_shared<IndexBuffer>(DXGI_FORMAT_R16_UINT, m_CubeIndices.size() * sizeof(unsigned short), m_CubeIndices.data());

		details.m_InputLayout = InputLayoutSystem::GetInputLayout(CUBE_LAYOUT);
		details.m_Name = "Cube";

		return details;
	}
}
