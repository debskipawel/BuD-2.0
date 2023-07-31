#pragma once

#include <vector>

#include <Objects/MeshDetails.h>

namespace BuD
{
	namespace Internal
	{
		class BaseMeshPrimitiveLoader
		{
		public:
			virtual ~BaseMeshPrimitiveLoader() = default;

			virtual MeshDetails LoadMesh() = 0;
		};
	}
}
