#pragma once

#include "BaseGuiLayer.h"

#include <ViewModels/MainViewModel.h>

class MainGuiLayer : public BaseGuiLayer
{
public:
	MainGuiLayer(MainViewModel& viewModel);

	virtual void DrawGui() override;

protected:

	MainViewModel& m_ViewModel;
	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
