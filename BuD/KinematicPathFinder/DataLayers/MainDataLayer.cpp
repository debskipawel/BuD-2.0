#include "MainDataLayer.h"

#include <numbers>

MainDataLayer::MainDataLayer()
	: m_AngularVelocity(5.0f)
{
}

auto MainDataLayer::Update(float deltaTime) -> void
{
	auto appState = m_AppStateDataLayer.m_AppState;

	if (appState != AppState::ANIMATION_RUNNING)
	{
		return;
	}

	if (m_SimulationDataLayer.IsFinished())
	{
		m_AppStateDataLayer.m_AppState = AppState::EDIT_START;
		return;
	}

	m_SimulationDataLayer.Update(deltaTime);

	auto configuration = m_SimulationDataLayer.GetConfiguration();

	m_SceneDataLayer.UpdateMeshes(configuration);
}
