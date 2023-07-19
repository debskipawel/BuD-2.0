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

		const std::vector<MeshSegment>& LoadMeshFromFile(std::filesystem::path filepath);
		const MeshDetails& LoadPrimitiveMesh(MeshPrimitiveType meshType);

	private:
		static std::map<std::filesystem::path, std::vector<MeshSegment>> s_FileMeshes;
		static std::map<MeshPrimitiveType, MeshDetails> s_PrimitiveMeshes;
	};
}
