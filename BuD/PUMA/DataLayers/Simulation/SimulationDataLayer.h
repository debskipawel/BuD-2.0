#pragma once

#include <BuD.h>

#include <DataLayers/Viewport/ViewportDataLayer.h>

class SimulationDataLayer
{
public:
	SimulationDataLayer();

	virtual auto Start() -> void;
	virtual auto Stop() -> void;

	virtual auto Render() -> void;

	virtual auto Camera() const -> std::shared_ptr<BuD::AbstractCamera>;

	virtual auto Update(float deltaTime) -> void;

	virtual auto ViewportDetails() -> ViewportDataLayer&;

protected:
	BuD::Scene m_Scene;

	ViewportDataLayer m_Viewport;

	bool m_Running;
};
