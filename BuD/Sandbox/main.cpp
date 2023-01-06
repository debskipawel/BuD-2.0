#include <BuD.h>
#include <Core/EntryPoint.h>

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
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
