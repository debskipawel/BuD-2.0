#pragma once

#include <filesystem>
#include <vector>

#include "MeshSegment.h"

namespace BuD
{
	class MeshLoader
	{
	public:
		MeshLoader() = default;

		std::vector<MeshSegment> LoadMesh(std::filesystem::path filepath);
	};
}
