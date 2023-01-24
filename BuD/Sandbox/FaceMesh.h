#pragma once

#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

class FaceMesh : public BuD::SceneEntity, public BuD::IRenderable, public BuD::IState 
{
public:
	FaceMesh(BuD::Scene& scene);
};
