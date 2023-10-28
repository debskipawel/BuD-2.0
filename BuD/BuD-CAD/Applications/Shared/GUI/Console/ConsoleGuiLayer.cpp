#include "ConsoleGuiLayer.h"

#include <imgui.h>
#include <codecvt>

ConsoleGuiLayer::ConsoleGuiLayer(std::string panelId)
	: m_PanelId(panelId)
{
	BuD::Log::RegisterLogHandle([this](const BuD::Log::LogRecord& record) 
		{
			m_LogRecords.push_back(record);
			m_NewLineAdded = true;
		}
	);
}

void ConsoleGuiLayer::DrawGui()
{
	static std::unordered_map<BuD::LogSeverity, ImVec4> severityColorMap =
	{
		{ BuD::LogSeverity::LS_INFO, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ BuD::LogSeverity::LS_WARNING, { 1.0f, 0.8f, 0.4f, 1.0f } },
		{ BuD::LogSeverity::LS_ERROR, { 1.0f, 0.1f, 0.1f, 1.0f } },
	};

	static std::unordered_map<BuD::LogSeverity, std::string> severityPrefixMap =
	{
		{ BuD::LogSeverity::LS_INFO, "[INFO]   " },
		{ BuD::LogSeverity::LS_WARNING, "[WARNING]" },
		{ BuD::LogSeverity::LS_ERROR, "[ERROR]  " },
	};

	auto panelLabel = std::format("Console ###{}", m_PanelId);

	if (ImGui::Begin(panelLabel.c_str()))
	{
		ImGuiListClipper clipper(m_LogRecords.size(), ImGui::GetTextLineHeightWithSpacing());

		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				auto& record = m_LogRecords[i];
				auto& color = severityColorMap[record.severity];
				auto& prefix = severityPrefixMap[record.severity];

				ImGui::TextColored(color, prefix.c_str());
				ImGui::SameLine();
				ImGui::TextWrapped(record.message.c_str());
			}
		}

		if (m_NewLineAdded)
		{
			ImGui::SetScrollHereY(1.0f);
		}

		m_NewLineAdded = false;

		ImGui::End();
	}
}
