#pragma once

#include <GUI/BaseGuiLayer.h>

class MainGuiLayer : public BaseGuiLayer
{
public:
	MainGuiLayer(MainDataLayer& viewModel);
	virtual ~MainGuiLayer() = default;

	virtual void DrawGui() override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
