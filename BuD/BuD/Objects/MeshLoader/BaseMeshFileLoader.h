#pragma once

#include <filesystem>
#include <vector>

#include "MeshSegment.h"

namespace BuD
{
	namespace Internal
	{
		class BaseMeshFileLoader
		{
		public:
			virtual ~BaseMeshFileLoader() = default;

			virtual std::vector<MeshSegment> LoadMesh(std::filesystem::path filepath) = 0;
		};
	}
}
