#include "AppStateGuiLayer.h"

#include <imgui.h>

AppStateGuiLayer::AppStateGuiLayer()
{

}

auto AppStateGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("AppState"))
	{
		ImGui::End();
	}
}
