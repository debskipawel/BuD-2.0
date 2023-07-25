#pragma once

#include <filesystem>
#include <vector>
#include <map>

#include "MeshSegment.h"

#include <Objects/MeshPrimitiveType.h>
#include <Objects/MeshDetails.h>

namespace BuD
{
	class MeshLoader
	{
	public:
		MeshLoader() = default;

		std::vector<MeshSegment> LoadMeshFromFile(std::filesystem::path filepath, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> instanceDataDescription = {});
		MeshDetails LoadPrimitiveMesh(MeshPrimitiveType meshType, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> instanceDataDescription = {});

	private:

		std::vector<MeshSegment> LoadMeshFromFileInternal(std::filesystem::path filepath);
		MeshDetails LoadPrimitiveMeshInternal(MeshPrimitiveType meshType);

		static std::unordered_map<std::filesystem::path, std::vector<MeshSegment>> s_FileMeshes;
		static std::unordered_map<MeshPrimitiveType, MeshDetails> s_PrimitiveMeshes;
	};
}
