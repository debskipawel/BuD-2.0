#include "BaseSimulation.h"

BaseSimulation::BaseSimulation()
    : m_Scene(), m_Viewport()
{
}

auto BaseSimulation::Render() -> void
{
	BuD::Renderer::BeginTarget(m_Viewport.m_ViewportWidth, m_Viewport.m_ViewportHeight);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
	BuD::Renderer::Render(m_Scene.Scene());

	m_Viewport.m_ViewportImage = BuD::Renderer::EndTarget();
}

auto BaseSimulation::Camera() -> std::shared_ptr<BuD::AbstractCamera>
{
	return m_Scene.Scene().ActiveCamera();
}

auto BaseSimulation::Update(const RobotParameters& robotParameters, const AnimationClip& animationClip, float time) -> void
{
	const auto& frames = animationClip.KeyFrames();

	if (frames.empty())
	{
		m_PreviousConfiguration = RobotConfiguration();
		m_PreviousConfigurationTime = time;

		return;
	}

	auto& front = frames.front();
	
	if (time <= front.GetTime())
	{
		m_PreviousConfiguration = front.GetConfiguration();
		m_PreviousConfigurationTime = time;

		m_Scene.UpdateRobotMesh(robotParameters, m_PreviousConfiguration);

		return;
	}

	auto& back = frames.back();

	if (time >= back.GetTime())
	{
		m_PreviousConfiguration = back.GetConfiguration();
		m_PreviousConfigurationTime = time;

		m_Scene.UpdateRobotMesh(robotParameters, m_PreviousConfiguration);

		return;
	}

	auto nextFrameIt = std::lower_bound(frames.begin(), frames.end(), time,
		[](const AnimationKeyFrame& keyFrame, float time)
		{
			return keyFrame.GetTime() < time;
		}
	);

	auto previousFrameIt = nextFrameIt - 1;

	const auto& previousFrame = *previousFrameIt;
	const auto& nextFrame = *nextFrameIt;

	auto t = (time - previousFrame.GetTime()) / (nextFrame.GetTime() - previousFrame.GetTime());

	auto configuration = Interpolate(robotParameters, previousFrame, nextFrame, t);

	m_PreviousConfiguration = configuration;
	m_PreviousConfigurationTime = time;

	m_Scene.UpdateRobotMesh(robotParameters, m_PreviousConfiguration);
}

auto BaseSimulation::ViewportDetails() -> ViewportDataLayer&
{
    return m_Viewport;
}
