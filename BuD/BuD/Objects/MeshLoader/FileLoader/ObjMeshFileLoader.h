#pragma once

#include "BaseMeshFileLoader.h"

namespace BuD
{
	namespace Internal
	{
		class ObjMeshFileLoader : public BaseMeshFileLoader
		{
		public:
			virtual std::vector<MeshSegment> LoadMesh(std::filesystem::path filepath) override;
		};
	}
}
