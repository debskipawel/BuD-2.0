#include "MillingSimulatorAsync.h"

#include <Applications/CNC/Simulator/MillingSimulatorThread.h>

MillingSimulatorAsync::MillingSimulatorAsync()
	: m_MaterialParameters(MaterialBlockParameters::DEFAULT_PARAMETERS), m_Running(false), m_TimeLeft()
{
	ResetMaterial(m_MaterialParameters);
}

MillingSimulatorAsync::~MillingSimulatorAsync()
{
	Stop();
}

void MillingSimulatorAsync::UploadPath(std::shared_ptr<PathProgram> pathProgram)
{
	if (m_Running)
	{
		return;
	}

	m_UploadedProgram = pathProgram;
}

void MillingSimulatorAsync::ResetMaterial(const MaterialBlockParameters& materialParameters)
{
	auto width = materialParameters.m_ResolutionWidth;
	auto height = materialParameters.m_ResolutionHeight;

	auto pixels = 4 * width * height;

	auto baseHeight = materialParameters.m_Size.y;

	m_HeightMap.resize(pixels);

	std::fill(m_HeightMap.begin(), m_HeightMap.end(), baseHeight);

	m_MaterialParameters = materialParameters;
}

void MillingSimulatorAsync::Update(float deltaTime)
{
	if (this->Running())
	{
		auto lockGuard = std::lock_guard<std::mutex>(m_TimeLeftMutex);

		m_TimeLeft += deltaTime;
	}
}

void MillingSimulatorAsync::Skip()
{
	if (!this->Running())
	{
		this->Start();
	}

	m_ToSkip = true;
}

void MillingSimulatorAsync::SimulationLoop()
{
	auto simulationThread = std::make_unique<MillingSimulatorThread>(m_MaterialParameters, m_UploadedProgram, std::ref(m_HeightMap));

	while (!simulationThread->Finished() && !this->Stopped())
	{
		auto deltaTime = 0.0f;
		
		if (m_ToSkip)
		{
			deltaTime = 10.0f;
		}
		else
		{
			auto lockGuard = std::lock_guard<std::mutex>(m_TimeLeftMutex);

			deltaTime = m_TimeLeft;
			m_TimeLeft = 0.0f;
		}

		if (!simulationThread->Update(deltaTime))
		{
			m_Running = false;

			return;
		}
	}

	if (simulationThread->Finished())
	{
		m_Running = false;
	}
}

void MillingSimulatorAsync::Start()
{
	if (this->Running())
	{
		return;
	}

	m_Running = true;
	m_ToSkip = false;

	BuD::Log::WriteInfo("Milling simulation started.");

	m_SimulatorThread = std::jthread([this]() { this->SimulationLoop(); });
}

void MillingSimulatorAsync::Stop()
{
	if (this->Stopped())
	{
		return;
	}

	m_Running = false;

	m_SimulatorThread.join();

	BuD::Log::WriteInfo("Milling simulation stopped.");
}
