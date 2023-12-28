#include "SimulationDataLayer.h"

SimulationDataLayer::SimulationDataLayer()
    : m_Scene(), m_Viewport()
{

}

auto SimulationDataLayer::Render() -> void
{
	BuD::Renderer::BeginTarget(m_Viewport.m_ViewportWidth, m_Viewport.m_ViewportHeight);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
	BuD::Renderer::Render(m_Scene.Scene());

	m_Viewport.m_ViewportImage = BuD::Renderer::EndTarget();
}

auto SimulationDataLayer::Camera() -> std::shared_ptr<BuD::AbstractCamera>
{
	return m_Scene.Scene().ActiveCamera();
}

auto SimulationDataLayer::Update(float deltaTime) -> void
{

}

auto SimulationDataLayer::ViewportDetails() -> ViewportDataLayer&
{
    return m_Viewport;
}
