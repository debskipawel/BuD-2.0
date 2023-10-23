#include "StartingParametersGuiLayer.h"

#include <imgui.h>

StartingParametersGuiLayer::StartingParametersGuiLayer(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
}

void StartingParametersGuiLayer::DrawGui()
{
	if (ImGui::Begin("Starting parameters ###starting_parameters"))
	{
		ImGui::End();
	}
}
