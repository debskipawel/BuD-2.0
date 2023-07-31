#pragma once

#include "BaseMeshPrimitiveLoader.h"

namespace BuD
{
	namespace Internal
	{
		class PointMeshPrimitiveLoader : public BaseMeshPrimitiveLoader
		{
		public:
			virtual MeshDetails LoadMesh() override;
		};
	}
}
