#include "bud_pch.h"
#include "MeshLoader.h"

#include <Objects/MeshLoader/FileLoader/ObjMeshFileLoader.h>

#include <Objects/MeshLoader/PrimitiveLoader/CubeMeshPrimitiveLoader.h>
#include <Objects/MeshLoader/PrimitiveLoader/QuadMeshPrimitiveLoader.h>

namespace BuD
{
	static const std::filesystem::path OBJ_EXTENSION = ".obj";

	static const std::vector<MeshSegment> EMPTY = {};

	const std::vector<MeshSegment>& MeshLoader::LoadMeshFromFile(std::filesystem::path filepath)
	{
		auto searchResult = s_FileMeshes.find(filepath);

		if (searchResult != s_FileMeshes.end())
		{
			return searchResult->second;
		}

		std::unique_ptr<Internal::BaseMeshFileLoader> fileLoader;

		if (filepath.extension() == OBJ_EXTENSION)
		{
			fileLoader = std::make_unique<Internal::ObjMeshFileLoader>();
		}
		else
		{
			Log::WriteWarning(L"Trying to load static mesh in " + filepath.extension().wstring() + L" format, which is incorrect or not currently supported.");
			return EMPTY;
		}

		auto staticMesh = fileLoader->LoadMesh(filepath);

		auto [insertIter, insertCorrect] = s_FileMeshes.insert(std::make_pair(filepath, staticMesh));

		return insertIter->second;
	}

	const MeshDetails& MeshLoader::LoadPrimitiveMesh(MeshPrimitiveType meshType)
	{
		auto searchResult = s_PrimitiveMeshes.find(meshType);

		if (searchResult != s_PrimitiveMeshes.end())
		{
			return searchResult->second;
		}

		std::unique_ptr<Internal::BaseMeshPrimitiveLoader> primitiveLoader;

		switch (meshType)
		{
		case BuD::MeshPrimitiveType::QUAD:
			primitiveLoader = std::make_unique<Internal::QuadMeshPrimitiveLoader>();
			break;
		case BuD::MeshPrimitiveType::CUBE:
			primitiveLoader = std::make_unique<Internal::CubeMeshPrimitiveLoader>();
			break;
		default:
			break;
		}

		auto staticMesh = primitiveLoader->LoadMesh();

		auto [insertIter, insertCorrect] = s_PrimitiveMeshes.insert(std::make_pair(meshType, staticMesh));

		return insertIter->second;
	}

	std::map<std::filesystem::path, std::vector<MeshSegment>> MeshLoader::s_FileMeshes = {};
	std::map<MeshPrimitiveType, MeshDetails> MeshLoader::s_PrimitiveMeshes = {};
}
