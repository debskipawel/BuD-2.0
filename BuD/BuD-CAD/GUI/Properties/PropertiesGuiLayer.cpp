#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include "PerformanceGuiLayer.h"
#include "ObjectGuiDrawerVisitor.h"

PropertiesGuiLayer::PropertiesGuiLayer(PropertiesViewModel& viewModel)
	: m_ViewModel(viewModel)
{
	m_PerformanceGuiLayer = std::make_unique<PerformanceGuiLayer>();
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		auto& composite = m_ViewModel.m_SelectedObjects;

		switch (composite.m_Objects.size())
		{
		case 0:
			break;
		case 1:
			DrawGuiForSingularObject();
			break;
		default:
			DrawGuiForComposite();
			break;
		}

		m_PerformanceGuiLayer->DrawGui();

		ImGui::End();
	}
}

void PropertiesGuiLayer::DrawGuiForSingularObject()
{
	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectGuiDrawerVisitor>();
	auto& composite = m_ViewModel.m_SelectedObjects;

	auto& [key, object] = *composite.m_Objects.begin();

	visitor->Visit(*object);
}

void PropertiesGuiLayer::DrawGuiForComposite()
{
}
