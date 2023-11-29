#pragma once

#include <BuD.h>

class KinematicPathFinderApp : public BuD::AppLayer
{
public:
	KinematicPathFinderApp();

	virtual auto OnUpdate(float deltaTime) -> void override;
	virtual auto OnGuiRender() -> void override;
	virtual auto OnRender() -> void override;
};
