#include "SimulationDataLayerCNC.h"

SimulationDataLayerCNC::SimulationDataLayerCNC()
	: m_SimulationSpeed(1.0f), m_MillingSimulator()
{
}


void SimulationDataLayerCNC::Update(float deltaTime)
{
	if (!Running())
	{
		return;
	}

	auto scaledDeltaTime = m_SimulationSpeed * deltaTime;

	m_MillingSimulator.Update(scaledDeltaTime);
}

void SimulationDataLayerCNC::StartSimulation()
{
	m_MillingSimulator.Start();
}

void SimulationDataLayerCNC::StopSimulation()
{
	m_MillingSimulator.Stop();
}

bool SimulationDataLayerCNC::Running() const
{
	return m_MillingSimulator.Running();
}

auto SimulationDataLayerCNC::GetSelectedPath() const -> std::shared_ptr<PathProgram>
{
	return m_SelectedPath;
}

auto SimulationDataLayerCNC::SetSelectedPath(std::shared_ptr<PathProgram> selectedPath) -> void
{
	if (Running())
	{
		return;
	}
	
	if (m_SelectedPath)
	{
		m_SelectedPath->m_Tool->DisableRendering();
	}
	
	m_SelectedPath = selectedPath;

	if (m_SelectedPath)
	{
		m_SelectedPath->m_Tool->EnableRendering();
	}

	m_MillingSimulator.UploadPath(selectedPath);
}
