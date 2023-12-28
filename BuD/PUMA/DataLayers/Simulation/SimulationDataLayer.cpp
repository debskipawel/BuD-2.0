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

auto SimulationDataLayer::Update(const RobotParameters& robotParameters, const AnimationClip& animationClip, float time) -> void
{
	const auto& frames = animationClip.KeyFrames();

	if (frames.empty())
	{
		return;
	}

	auto& front = frames.front();
	
	if (time <= front.Time())
	{
		m_Scene.UpdateRobotMesh(robotParameters, front.Configuration());

		return;
	}

	auto& back = frames.back();

	if (time >= back.Time())
	{
		m_Scene.UpdateRobotMesh(robotParameters, back.Configuration());

		return;
	}

	auto previousFrameIt = std::lower_bound(frames.begin(), frames.end(), time,
		[](const AnimationKeyFrame& keyFrame, float time)
		{
			return keyFrame.Time() < time;
		}
	);

	auto nextFrameIt = previousFrameIt + 1;

	const auto& previousFrame = *previousFrameIt;
	const auto& nextFrame = *nextFrameIt;

	auto t = (time - previousFrame.Time()) / (nextFrame.Time() - previousFrame.Time());

	auto configuration = Interpolate(previousFrame, nextFrame, t);

	m_Scene.UpdateRobotMesh(robotParameters, configuration);
}

auto SimulationDataLayer::ViewportDetails() -> ViewportDataLayer&
{
    return m_Viewport;
}
