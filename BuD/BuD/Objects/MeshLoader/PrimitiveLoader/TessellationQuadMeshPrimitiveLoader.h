#pragma once

#include "BaseMeshPrimitiveLoader.h"

namespace BuD
{
	namespace Internal
	{
		class TessellationQuadMeshPrimitiveLoader : public BaseMeshPrimitiveLoader
		{
		public:
			virtual MeshDetails LoadMesh() override;
		};
	}
}
