#include "ApplicationCAD.h"

#include <string>

ApplicationCAD::ApplicationCAD()
{
	m_GuiLayer = std::make_unique<GuiLayerCAD>();

	BuD::Log::RegisterLogHandle(
		[](const BuD::Log::LogRecord& record)
		{
			auto message = std::format(L"[{}] {}\n", record.time.Format(), record.message);

			OutputDebugStringW(message.c_str());
		}
	);
}

void ApplicationCAD::OnUpdate(float deltaTime)
{
}

void ApplicationCAD::OnRender()
{
	auto& viewportVM = m_ViewModel.m_ViewportViewModel;

	BuD::Renderer::BeginTarget(viewportVM.m_ViewportWidth, viewportVM.m_ViewportHeight);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

	//BuD::Renderer::Render();

	viewportVM.m_ViewportImage = BuD::Renderer::EndTarget();
}

void ApplicationCAD::OnGuiRender()
{
	m_GuiLayer->DrawGui(m_ViewModel);
}
