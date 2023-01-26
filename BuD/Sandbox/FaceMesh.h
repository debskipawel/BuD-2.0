#pragma once

#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

class FaceMesh
{
public:
	FaceMesh(BuD::Scene& scene);

	bool m_TransmissionMapOn = true;
	bool m_SpecularMapOn = true;
	bool m_SSSMapOn = true;
	bool m_RoughnessMapOn = true;
	bool m_NormalMapOn = true;
	bool m_MicronormalMapOn = true;
	bool m_MicronormalMaskOn = true;
	bool m_DiffuseMapOn = true;
	bool m_AmbientOcclussionMapOn = true;
	bool m_DepthMapAbsorptionOn = true;

	float m_Grow;

	dxm::Vector3 m_PassingExpMultiplier = { 50.0f, 50.0f, 100.0f };

protected:

	void LoadTextures(std::string meshName);

	BuD::SceneEntity m_Entity;
	std::vector<BuD::RenderingPass> m_RenderingPasses;
	BuD::Texture m_LightDepthMap;

	BuD::Texture m_TransmissionMap;
	BuD::Texture m_SpecularMap;
	BuD::Texture m_SSSMap;
	BuD::Texture m_RoughnessMap;
	BuD::Texture m_NormalMap;
	BuD::Texture m_MicronormalMap;
	BuD::Texture m_MicronormalMask;
	BuD::Texture m_DiffuseMap;
	BuD::Texture m_AmbientOcclusionMap;
};
