#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/MaterialBlock.h>

struct SceneDataLayerCNC
{
	SceneDataLayerCNC()
		: m_Scene(), m_Material(m_Scene, {3.0f, 3.0f, 2.0f}, 1024, 1024)
	{
		
	}

	BuD::Scene m_Scene;
	MaterialBlock m_Material;
};
