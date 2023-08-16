#pragma once

#include "BaseMeshPrimitiveLoader.h"

#include <Objects/MeshPrimitiveType.h>

namespace BuD
{
	namespace Internal
	{
		class PointMeshPrimitiveLoader : public BaseMeshPrimitiveLoader
		{
		public:
			PointMeshPrimitiveLoader(BuD::MeshPrimitiveType meshType);

			virtual MeshDetails LoadMesh() override;

		protected:
			BuD::MeshPrimitiveType m_PrimitiveType;
		};
	}
}
