#include "MillingSimulator.h"

#include <Applications/CNC/Simulator/MillingSimulatorWorkerThread.h>

MillingSimulator::MillingSimulator()
	: m_MaterialParameters(MaterialBlockParameters::DEFAULT_PARAMETERS), m_Running(false), m_TimeLeft()
{
	ResetMaterial(m_MaterialParameters);
}

MillingSimulator::~MillingSimulator()
{
	Stop();
}

void MillingSimulator::UploadPath(std::shared_ptr<PathProgram> pathProgram)
{
	if (m_Running)
	{
		return;
	}

	m_UploadedProgram = pathProgram;
}

void MillingSimulator::ResetMaterial(const MaterialBlockParameters& materialParameters)
{
	auto width = materialParameters.m_ResolutionWidth;
	auto height = materialParameters.m_ResolutionHeight;

	auto pixels = 4 * width * height;

	auto baseHeight = materialParameters.m_Size.y;

	m_HeightMap.resize(pixels);

	std::fill(m_HeightMap.begin(), m_HeightMap.end(), baseHeight);

	m_MaterialParameters = materialParameters;
}

void MillingSimulator::Update(float deltaTime)
{
	if (this->Running())
	{
		auto lockGuard = std::lock_guard<std::mutex>(m_TimeLeftMutex);

		m_TimeLeft += deltaTime;
	}
}

void MillingSimulator::Skip()
{
	if (!this->Running())
	{
		this->Start();
	}

	m_ToSkip = true;
}

void MillingSimulator::SimulationLoop()
{
	auto simulationWorkerThread = std::make_unique<MillingSimulatorWorkerThread>(m_MaterialParameters, m_UploadedProgram, std::ref(m_HeightMap));

	while (!Stopped() && !simulationWorkerThread->Finished())
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

		if (!simulationWorkerThread->Update(deltaTime))
		{
			m_Running = false;

			return;
		}
	}

	if (simulationWorkerThread->Finished())
	{
		m_Running = false;
	}
}

void MillingSimulator::Start()
{
	if (this->Running())
	{
		return;
	}

	m_Running = true;
	m_ToSkip = false;

	m_TimeLeft = 0.0f;

	BuD::Log::WriteInfo("Milling simulation started.");

	m_SimulatorThread = std::jthread([this]() { this->SimulationLoop(); });
}

void MillingSimulator::Stop()
{
	if (this->Stopped())
	{
		return;
	}

	m_Running = false;

	m_SimulatorThread.join();

	BuD::Log::WriteInfo("Milling simulation stopped.");
}
