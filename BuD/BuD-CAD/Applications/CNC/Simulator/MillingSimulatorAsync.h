#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <Applications/CNC/PathProgram.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

class MillingSimulatorAsync
{
public:
	MillingSimulatorAsync();
	~MillingSimulatorAsync();

	virtual void UploadPath(std::shared_ptr<PathProgram> pathProgram);
	virtual void ResetMaterial(const MaterialBlockParameters& materialParameters);

	virtual const std::vector<float>& Results() const { return m_HeightMap; }

	virtual void Update(float deltaTime);
	virtual void Skip();

	virtual void SimulationLoop();

	virtual void Start();
	virtual void Stop();

	virtual constexpr bool Running() const { return m_Running; }
	virtual constexpr bool Stopped() const { return !m_Running; }

protected:
	std::vector<float> m_HeightMap;

	std::jthread m_SimulatorThread;

	std::shared_ptr<PathProgram> m_UploadedProgram;
	MaterialBlockParameters m_MaterialParameters;

	float m_TimeLeft;
	std::mutex m_TimeLeftMutex;

	bool m_Running;

	bool m_ToSkip;
};
