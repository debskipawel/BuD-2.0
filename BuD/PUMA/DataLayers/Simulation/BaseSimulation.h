#pragma once

#include <DataLayers/Viewport/ViewportDataLayer.h>
#include <Robot/Animation/AnimationClip.h>
#include <Scene/PumaScene.h>

class BaseSimulation
{
public:
	BaseSimulation();
	virtual ~BaseSimulation() = default;

	virtual auto Render() -> void;
	virtual auto Update(const RobotParameters& robotParameters, const AnimationClip& animationClip, float time) -> void;

	virtual auto ViewportDetails() -> ViewportDataLayer&;
	virtual auto Camera() -> std::shared_ptr<BuD::AbstractCamera>;

	virtual auto Name() -> std::string = 0;

protected:
	virtual auto Interpolate(const RobotParameters& robotParameters, const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration = 0;

	PumaScene m_Scene;

	ViewportDataLayer m_Viewport;
};
