#pragma once

#include <BuD.h>

#include <DataLayers/MainDataLayer.h>
#include <GUI/BaseGuiLayer.h>

class PumaApp : public BuD::AppLayer
{
public:
	PumaApp();

	virtual auto OnUpdate(float deltaTime) -> void override;

	virtual auto OnRender() -> void override;
	virtual auto OnGuiRender() -> void override;

protected:
	BuD::Scene m_Scene;
	MainDataLayer m_MainDataLayer;

	std::unique_ptr<BaseGuiLayer> m_MainGuiLayer;
};
