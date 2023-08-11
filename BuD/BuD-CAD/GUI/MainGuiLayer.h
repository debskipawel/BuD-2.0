#pragma once

#include "BaseGuiLayer.h"

#include <DataLayers/MainDataLayer.h>

class MainGuiLayer : public BaseGuiLayer
{
public:
	MainGuiLayer(MainDataLayer& viewModel);

	virtual void DrawGui() override;

protected:

	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
