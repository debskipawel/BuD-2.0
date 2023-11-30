#pragma once

#include <BuD.h>

#include <GUI/MainGuiLayer.h>

#include <DataLayers/MainDataLayer.h>

class KinematicPathFinderApp : public BuD::AppLayer
{
public:
	KinematicPathFinderApp();

	virtual auto OnUpdate(float deltaTime) -> void override;
	virtual auto OnGuiRender() -> void override;
	virtual auto OnRender() -> void override;

protected:
	MainDataLayer m_MainDataLayer;

	MainGuiLayer m_MainGuiLayer;
};
