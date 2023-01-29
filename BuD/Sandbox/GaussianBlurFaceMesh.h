#pragma once

#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

#include "FaceMesh.h"

class GaussianBlurFaceMesh : public FaceMesh
{
public:
	GaussianBlurFaceMesh(BuD::Scene& scene);

	BuD::Texture m_LightMap;
	BuD::Texture m_BakedLighting;
	BuD::Texture m_BlurredBakedIrradiance;
};

