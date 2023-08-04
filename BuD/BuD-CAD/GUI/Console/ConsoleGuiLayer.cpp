#include "ConsoleGuiLayer.h"

#include <imgui.h>

ConsoleGuiLayer::ConsoleGuiLayer()
{
}

void ConsoleGuiLayer::DrawGui()
{
	if (ImGui::Begin("Console"))
	{
		ImGui::Text("Here be dragons...");
		ImGui::Text("Logs should be displayed here (maybe someday).");

		ImGui::End();
	}
}
