#pragma once

#include <BuD.h>

struct SimulationDataLayer
{
	SimulationDataLayer();

	virtual void Run();
	virtual void Stop();

	virtual void Update(float deltaTime);
};
