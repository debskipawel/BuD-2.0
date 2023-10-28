#pragma once

#include <BuD.h>

#include <numbers>

#include <Applications/CNC/Objects/MaterialBlock/MaterialBlock.h>

struct SceneDataLayerCNC
{
	SceneDataLayerCNC();

	BuD::Scene m_Scene;
	MaterialBlock m_Material;
};
