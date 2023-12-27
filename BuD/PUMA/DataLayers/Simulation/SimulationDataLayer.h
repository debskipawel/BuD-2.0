#pragma once

#include <DataLayers/Viewport/ViewportDataLayer.h>
#include <Scene/PumaScene.h>

class SimulationDataLayer
{
public:
	SimulationDataLayer();

	virtual auto Start() -> void;
	virtual auto Stop() -> void;

	virtual auto Render() -> void;

	virtual auto Camera() -> std::shared_ptr<BuD::AbstractCamera>;

	virtual auto Update(float deltaTime) -> void;

	virtual auto ViewportDetails() -> ViewportDataLayer&;

protected:
	PumaScene m_Scene;

	ViewportDataLayer m_Viewport;

	bool m_Running;
};
