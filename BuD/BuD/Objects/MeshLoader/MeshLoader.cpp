#include "bud_pch.h"
#include "MeshLoader.h"

#include "ObjMeshFileLoader.h"

namespace BuD
{
	static const std::filesystem::path OBJ_EXTENSION = ".obj";

	std::vector<MeshSegment> MeshLoader::LoadMesh(std::filesystem::path filepath)
	{
		std::unique_ptr<BaseMeshFileLoader> fileLoader;

		if (filepath.extension() == OBJ_EXTENSION)
		{
			fileLoader = std::make_unique<ObjMeshFileLoader>();
		}
		else
		{
			Log::WriteWarning(L"Trying to load static mesh in " + filepath.extension().wstring() + L" format, which is incorrect or not currently supported.");
			return {};
		}

		auto staticMesh = fileLoader->LoadMesh(filepath);

		return staticMesh;
	}
}
