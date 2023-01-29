#include "FaceMesh.h"

FaceMesh::FaceMesh(BuD::Scene& scene)
{
	m_Entity = BuD::SceneEntity(scene);

	BuD::MeshLoader loader;
	m_MeshSegments = loader.LoadMesh("Resources/RogerHead.obj");

	LoadTextures(m_MeshSegments[0].SegmentName);
}

void FaceMesh::LoadTextures(std::string meshName)
{
	m_TransmissionMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/transmissionMap.png");
	m_SpecularMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/specularMap.jpg");
	m_SSSMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/SSSMap.jpg");
	m_RoughnessMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/roughnessMap.jpg");
	m_NormalMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/normalMap.png");
	m_MicronormalMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/mikroNormalMap.png");
	m_MicronormalMask = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/mikroNormalMask.jpg");
	m_DiffuseMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/diffuseMap.jpg");
	m_AmbientOcclusionMap = BuD::Texture::LoadFromFile("Resources/textures/" + meshName + "/ambientOcclusionMap.jpg");
}
