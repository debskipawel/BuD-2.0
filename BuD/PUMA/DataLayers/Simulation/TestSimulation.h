#pragma once

#include <DataLayers/Simulation/BaseSimulation.h>

class TestSimulation : public BaseSimulation
{
protected:
	virtual auto Interpolate(const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration override;
};
