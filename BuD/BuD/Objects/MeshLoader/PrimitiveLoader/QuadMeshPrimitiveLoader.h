#pragma once

#include "BaseMeshPrimitiveLoader.h"

namespace BuD
{
	namespace Internal
	{
		class QuadMeshPrimitiveLoader : public BaseMeshPrimitiveLoader
		{
		public:
			virtual MeshDetails LoadMesh() override;
		};
	}
}
