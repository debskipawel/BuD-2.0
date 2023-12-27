#pragma once

#include <BuD.h>

#include <Scene/Objects/Grid.h>

class PumaScene
{
public:
	PumaScene();

	virtual auto Scene() -> BuD::Scene&;

protected:
	BuD::Scene m_Scene;

	std::unique_ptr<Grid> m_Grid;
};
