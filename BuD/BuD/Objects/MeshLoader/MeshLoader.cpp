#include "bud_pch.h"
#include "MeshLoader.h"

#include <Layout/InputLayoutSystem.h>

#include <Objects/MeshLoader/FileLoader/ObjMeshFileLoader.h>

#include <Objects/MeshLoader/PrimitiveLoader/CubeMeshPrimitiveLoader.h>
#include <Objects/MeshLoader/PrimitiveLoader/PointMeshPrimitiveLoader.h>
#include <Objects/MeshLoader/PrimitiveLoader/QuadMeshPrimitiveLoader.h>

namespace BuD
{
	static const std::filesystem::path OBJ_EXTENSION = ".obj";

	static const std::vector<MeshSegment> EMPTY = {};

	std::vector<MeshSegment> MeshLoader::LoadMeshFromFile(std::filesystem::path filepath, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> instanceDataDescription)
	{
		auto meshElements = LoadMeshFromFileInternal(filepath);

		if (instanceDataDescription.size() == 0)
		{
			return meshElements;
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> instanceLayoutsElements; 
		instanceLayoutsElements.reserve(instanceDataDescription.size());

		for (auto instanceLayoutElement : instanceDataDescription)
		{
			instanceLayoutElement.InputSlot = 1;
			instanceLayoutElement.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			instanceLayoutElement.InstanceDataStepRate = 1;

			instanceLayoutsElements.push_back(instanceLayoutElement);
		}

		for (auto& meshElement : meshElements)
		{
			auto layout = meshElement.m_Details.m_InputLayout;
			auto layoutElements = layout->GetDescription().GetLayoutElements();

			auto currentSize = layoutElements.size();

			layoutElements.resize(currentSize + instanceLayoutsElements.size());

			std::memcpy(&layoutElements[currentSize], instanceLayoutsElements.data(), instanceLayoutsElements.size() * sizeof(D3D11_INPUT_ELEMENT_DESC));

			auto newLayout = InputLayoutSystem::GetInputLayout(layoutElements);

			meshElement.m_Details.m_InputLayout = newLayout;
		}

		return meshElements;
	}

	MeshDetails MeshLoader::LoadPrimitiveMesh(MeshPrimitiveType meshType, std::initializer_list<D3D11_INPUT_ELEMENT_DESC> instanceDataDescription)
	{
		auto meshDetails = LoadPrimitiveMeshInternal(meshType);

		if (instanceDataDescription.size() == 0)
		{
			return meshDetails;
		}

		auto& inputLayoutDescription = meshDetails.m_InputLayout->GetDescription();
		auto layoutElements = inputLayoutDescription.GetLayoutElements();

		std::vector<D3D11_INPUT_ELEMENT_DESC> instanceLayoutsElements;
		instanceLayoutsElements.reserve(instanceDataDescription.size());

		for (auto instanceLayoutElement : instanceDataDescription)
		{
			instanceLayoutElement.InputSlot = 1;
			instanceLayoutElement.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			instanceLayoutElement.InstanceDataStepRate = 1;

			instanceLayoutsElements.push_back(instanceLayoutElement);
		}

		auto currentSize = layoutElements.size();

		layoutElements.resize(currentSize + instanceLayoutsElements.size());

		std::memcpy(&layoutElements[currentSize], instanceLayoutsElements.data(), instanceLayoutsElements.size() * sizeof(D3D11_INPUT_ELEMENT_DESC));

		auto newLayout = InputLayoutSystem::GetInputLayout(layoutElements);

		meshDetails.m_InputLayout = newLayout;

		return meshDetails;
	}

	std::vector<MeshSegment> MeshLoader::LoadMeshFromFileInternal(std::filesystem::path filepath)
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
			Log::WriteWarning("Trying to load static mesh in " + filepath.extension().string() + " format, which is incorrect or not currently supported.");
			return EMPTY;
		}

		auto staticMesh = fileLoader->LoadMesh(filepath);

		auto [insertIter, insertCorrect] = s_FileMeshes.insert(std::make_pair(filepath, staticMesh));

		return insertIter->second;
	}

	MeshDetails MeshLoader::LoadPrimitiveMeshInternal(MeshPrimitiveType meshType)
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
		case BuD::MeshPrimitiveType::POINT_TESSELLATION:
		case BuD::MeshPrimitiveType::POINT_STANDARD:
			primitiveLoader = std::make_unique<Internal::PointMeshPrimitiveLoader>(meshType);
		default:
			break;
		}

		auto staticMesh = primitiveLoader->LoadMesh();

		auto [insertIter, insertCorrect] = s_PrimitiveMeshes.insert(std::make_pair(meshType, staticMesh));

		return insertIter->second;
	}

	std::unordered_map<std::filesystem::path, std::vector<MeshSegment>> MeshLoader::s_FileMeshes = {};
	std::unordered_map<MeshPrimitiveType, MeshDetails> MeshLoader::s_PrimitiveMeshes = {};
}
