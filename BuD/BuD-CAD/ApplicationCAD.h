#pragma once

#include <BuD.h>

class ApplicationCAD : public BuD::AppLayer
{
public:
	ApplicationCAD();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new ApplicationCAD());
}
