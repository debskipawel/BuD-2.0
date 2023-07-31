#include "BlackHoleQuadBase.h"

#include <Buffers/VertexBuffer.h>
#include <Buffers/IndexBuffer.h>
#include <Shaders/Loader/ShaderLoader.h>

#include <Objects/MeshLoader/MeshLoader.h>

BlackHoleQuadBase::BlackHoleQuadBase(BuD::Scene& scene)
	: m_BlackHoleMass(5.0f)
{
	m_GalaxySkybox = BuD::Texture::LoadFromFile("Resources/textures/galaxy_red.dds");
	m_LinusSkybox = BuD::Texture::LoadFromFile("Resources/textures/linus_cubemap.dds");

	m_BlackHoleEntity = BuD::SceneEntity(scene);
}
