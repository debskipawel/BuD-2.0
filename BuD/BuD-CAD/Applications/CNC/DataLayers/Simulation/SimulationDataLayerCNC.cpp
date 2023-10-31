#include "SimulationDataLayerCNC.h"

#include <numbers>

SimulationDataLayerCNC::SimulationDataLayerCNC()
	: m_Scene(), m_SimulationSpeed(1.0f), m_MillingSimulator(m_Scene)
{
	auto camera = m_Scene.ActiveCamera();
	camera->Zoom(20.0f);
	camera->RotateCamera(-std::numbers::pi_v<float> / 4.0f, std::numbers::pi_v<float> / 6.0f);
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

void SimulationDataLayerCNC::ResetMaterial(const MaterialBlockParameters& materialParameters, uint32_t resolutionWidth, uint32_t resolutionHeight)
{
	m_MillingSimulator.ResetMaterial(materialParameters, resolutionWidth, resolutionHeight);
}

void SimulationDataLayerCNC::StartSimulation()
{
	m_MillingSimulator.Start();
}

void SimulationDataLayerCNC::StopSimulation()
{
	m_MillingSimulator.Stop();
}

void SimulationDataLayerCNC::SkipSimulation()
{
	while (Running())
	{
		m_MillingSimulator.Update(10.0f);
	}
}

bool SimulationDataLayerCNC::Running() const
{
	return m_MillingSimulator.Running();
}

bool SimulationDataLayerCNC::Paused() const
{
	return m_MillingSimulator.Paused();
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
