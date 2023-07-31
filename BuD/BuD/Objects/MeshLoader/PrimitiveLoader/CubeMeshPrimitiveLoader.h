#pragma once

#include "BaseMeshPrimitiveLoader.h"

namespace BuD
{
	namespace Internal
	{
		class CubeMeshPrimitiveLoader : public BaseMeshPrimitiveLoader
		{
		public:
			virtual MeshDetails LoadMesh() override;
		};
	}
}
