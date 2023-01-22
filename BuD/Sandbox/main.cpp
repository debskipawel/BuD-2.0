#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Scene/Scene.h>
#include <Utils/UUID.h>

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
	}

protected:
	int m_Counter;
	BuD::Scene m_Scene;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
