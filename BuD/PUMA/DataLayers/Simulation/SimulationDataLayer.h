#pragma once

#include <DataLayers/Viewport/ViewportDataLayer.h>
#include <Scene/PumaScene.h>

class SimulationDataLayer
{
public:
	SimulationDataLayer();

	virtual auto Render() -> void;
	virtual auto Update(float deltaTime) -> void;

	virtual auto ViewportDetails() -> ViewportDataLayer&;
	virtual auto Camera() -> std::shared_ptr<BuD::AbstractCamera>;

protected:
	PumaScene m_Scene;

	ViewportDataLayer m_Viewport;
};
