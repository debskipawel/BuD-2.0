#pragma once

#include <DataLayers/Simulation/BaseSimulation.h>
#include <Robot/Frame.h>

class FrameInterpolationSimulation : public BaseSimulation
{
public:
	FrameInterpolationSimulation();
	virtual ~FrameInterpolationSimulation() = default;

protected:
	virtual auto Interpolate(const RobotParameters& parameters, const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration override;

	virtual auto InterpolateFrame(const Frame& frame1, const Frame& frame2, float t) -> Frame;
};
