#pragma once

#include <Renderer/Texture.h>
#include <Renderer/Renderer.h>
#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

enum ActiveCubemap
{
	GALAXY = 0,
	LINUS = 1
};

class BlackHoleQuadBase
{
public:
	BlackHoleQuadBase(BuD::Scene& scene);

	virtual dxm::Vector3 Position() const { return m_BlackHolePosition; }

	float m_BlackHoleMass;

	ActiveCubemap m_ActiveCubemap;

protected:
	BuD::SceneEntity m_BlackHoleEntity;

	BuD::Texture m_GalaxySkybox;
	BuD::Texture m_LinusSkybox;

	dxm::Vector3 m_BlackHolePosition = dxm::Vector3::Zero;
};
