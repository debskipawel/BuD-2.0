#include "MillingSimulatorAsync.h"

#include <Applications/CNC/Simulator/MillingSimulatorThread.h>

MillingSimulatorAsync::MillingSimulatorAsync()
	: m_MaterialParameters(MaterialBlockParameters::DEFAULT_PARAMETERS), m_Running(false), m_Paused(false), m_TimeLeft()
{
	ResetMaterial(m_MaterialParameters);
}

MillingSimulatorAsync::~MillingSimulatorAsync()
{
	this->Stop();
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
	try
	{
		auto width = materialParameters.m_ResolutionWidth;
		auto height = materialParameters.m_ResolutionHeight;

		auto pixels = 4 * width * height;

		auto baseHeight = m_MaterialParameters.m_Size.y;

		m_HeightMap.resize(pixels);

		std::fill(m_HeightMap.begin(), m_HeightMap.end(), baseHeight);
	}
	catch (std::bad_alloc e)
	{
		BuD::Log::WriteError("Bad alloc exception while resizing material height map");
	}

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
	if (!this->Running() && !this->Paused())
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

		simulationThread->Update(deltaTime);
	}

	if (simulationThread->Finished())
	{
		m_Running = false;
	}
}

void MillingSimulatorAsync::Start()
{
	if (this->Running() || this->Paused())
	{
		return;
	}

	m_Running = true;
	m_Paused = false;

	m_ToSkip = false;

	m_SimulatorThread = std::jthread([this]() { this->SimulationLoop(); });
}

void MillingSimulatorAsync::Pause()
{
	if (this->Stopped())
	{
		return;
	}

	m_Paused = true;
}

void MillingSimulatorAsync::Resume()
{
	if (!this->Paused())
	{
		return;
	}

	m_Paused = false;
}

void MillingSimulatorAsync::Stop()
{
	if (this->Stopped())
	{
		return;
	}

	m_Running = false;

	m_SimulatorThread.join();
}
