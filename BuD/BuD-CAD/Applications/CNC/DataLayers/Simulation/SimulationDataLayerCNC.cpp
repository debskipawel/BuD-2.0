#include "SimulationDataLayerCNC.h"

#include <numbers>

SimulationDataLayerCNC::SimulationDataLayerCNC()
	: m_Scene(), m_SimulationSpeed(1.0f), m_MillingSimulator(), m_MaterialBlockMesh(m_Scene, MaterialBlockParameters::DEFAULT_PARAMETERS)
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

	// TODO: update the heightmap texture based on data from the simulation
	auto heightMap = m_MaterialBlockMesh.HeightMap();

	heightMap->BeginEdit();

	heightMap->CopyFromBuffer(m_MillingSimulator.Results());

	heightMap->EndEdit();
}

void SimulationDataLayerCNC::ResetMaterial(const MaterialBlockParameters& materialParameters)
{
	m_MillingSimulator.ResetMaterial(materialParameters);

	m_MaterialBlockMesh = MaterialBlock(m_Scene, materialParameters);
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
	m_MillingSimulator.Skip();
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
