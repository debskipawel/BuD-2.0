#pragma once

#include "FaceMesh.h"

class DepthMapFaceMesh : public FaceMesh
{
public:
	DepthMapFaceMesh(BuD::Scene& scene);

	float m_Grow;

	dxm::Vector3 m_PassingExpMultiplier = { 50.0f, 50.0f, 100.0f };

	BuD::Texture m_LightDepthMap;
};
