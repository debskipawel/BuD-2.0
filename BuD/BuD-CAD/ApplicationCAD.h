#pragma once

#include <BuD.h>
#include <Core/EntryPoint.h>

#include "GuiLayerCAD.h"

#include <ViewModels/MainViewModel.h>

class ApplicationCAD : public BuD::AppLayer
{
public:
	ApplicationCAD();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

private:
	std::unique_ptr<GuiLayerCAD> m_GuiLayer;

	MainViewModel m_ViewModel;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new ApplicationCAD());
}
