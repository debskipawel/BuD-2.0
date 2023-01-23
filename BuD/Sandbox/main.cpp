#include <BuD.h>
#include <Core/EntryPoint.h>

#include "imgui.h"

class SandboxApp : public BuD::AppLayer
{
public:
	SandboxApp() = default;

	void OnUpdate(float deltaTime) override
	{
		m_Counter++;
	}

	void OnRender() override
	{
		m_Counter++;
	}

	void OnGuiRender() override
	{
		ImGui::ShowDemoWindow(&m_ShowDemoWindow);
	}

protected:
	bool m_ShowDemoWindow = true;
	int m_Counter;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
