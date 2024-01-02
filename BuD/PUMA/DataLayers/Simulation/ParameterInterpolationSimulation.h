#pragma once

#include <DataLayers/Simulation/BaseSimulation.h>

class ParameterInterpolationSimulation : public BaseSimulation
{
public:
	ParameterInterpolationSimulation();
	virtual ~ParameterInterpolationSimulation() = default;

protected:
	virtual auto Interpolate(const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration override;
};
