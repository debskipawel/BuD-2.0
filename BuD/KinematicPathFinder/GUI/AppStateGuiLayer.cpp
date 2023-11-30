#include "AppStateGuiLayer.h"

#include <imgui.h>

AppStateGuiLayer::AppStateGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{

}

auto AppStateGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("AppState"))
	{
		ImGui::End();
	}
}
