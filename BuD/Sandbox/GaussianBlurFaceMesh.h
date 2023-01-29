#pragma once

#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

#include "FaceMesh.h"

class GaussianBlurFaceMesh : public FaceMesh
{
public:
	GaussianBlurFaceMesh(BuD::Scene& scene);

protected:
	BuD::Texture m_BlurredUVMap;
};

