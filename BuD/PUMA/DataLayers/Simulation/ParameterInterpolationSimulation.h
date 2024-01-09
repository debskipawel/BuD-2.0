#pragma once

#include <DataLayers/Simulation/BaseSimulation.h>

class ParameterInterpolationSimulation : public BaseSimulation
{
public:
	ParameterInterpolationSimulation();
	virtual ~ParameterInterpolationSimulation() = default;

	virtual auto Name() -> std::string override;

protected:
	virtual auto Interpolate(const RobotParameters& parameters, const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration override;
};
