#pragma once

#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

enum ActiveCubemap
{
	GALAXY = 0,
	LINUS = 1
};

class BlackHoleQuad
{
public:
	BlackHoleQuad(BuD::Scene& scene);

	dxm::Vector3 Position() const { return m_BlackHolePosition; }

	float m_BlackHoleMass;

	ActiveCubemap m_ActiveCubemap;

private:
	BuD::SceneEntity m_BlackHoleEntity;

	BuD::Texture m_GalaxySkybox;
	BuD::Texture m_LinusSkybox;
	
	dxm::Vector3 m_BlackHolePosition = dxm::Vector3::Zero;
};
