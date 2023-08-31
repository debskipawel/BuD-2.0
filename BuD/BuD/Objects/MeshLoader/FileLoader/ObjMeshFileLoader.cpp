#include "bud_pch.h"
#include "ObjMeshFileLoader.h"

#include <Layout/InputLayoutSystem.h>
#include <Objects/MeshLoader/MeshSegment.h>

#include <d3d11.h>
#include <OBJ_Loader.h>

namespace BuD::Internal
{
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> OBJ_FILE_LAYOUT
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 6 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 9 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	std::vector<MeshSegment> ObjMeshFileLoader::LoadMesh(std::filesystem::path filepath)
	{
		objl::Loader Loader;
		bool loaded = Loader.LoadFile(filepath.string());
		
		if (!loaded)
		{
			Log::WriteError("Error while loading an .obj static mesh.");
			return {};
		}

		std::vector<MeshSegment> meshSegments;

		for (auto& mesh : Loader.LoadedMeshes)
		{
			MeshSegment segment{};

			auto& vertices = mesh.Vertices;
			auto& indices = mesh.Indices;
			auto& material = mesh.MeshMaterial;

			segment.m_Details.m_Name = mesh.MeshName;
			segment.m_Details.m_InputLayout = InputLayoutSystem::GetInputLayout(OBJ_FILE_LAYOUT);

			segment.m_Material = MeshMaterial
			{
				{ material.Ka.X, material.Ka.Y, material.Ka.Z }, 
				{ material.Kd.X, material.Kd.Y, material.Kd.Z }, 
				{ material.Ks.X, material.Ks.Y, material.Ks.Z },
				material.Ns, material.Ni, material.d, material.illum
			};

			segment.m_Details.m_VertexBuffer = std::make_shared<VertexBuffer>(vertices.size() * sizeof(objl::Vertex), OBJ_FILE_LAYOUT, vertices.data());
			segment.m_Details.m_IndexBuffer = std::make_shared<IndexBuffer>(DXGI_FORMAT_R32_UINT, indices.size() * sizeof(unsigned int), indices.data());

			meshSegments.push_back(segment);
		}

		return meshSegments;
	}
}
