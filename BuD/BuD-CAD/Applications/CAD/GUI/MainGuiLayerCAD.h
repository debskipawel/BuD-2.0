#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

#include <Applications/CAD/DataLayers/MainDataLayer.h>

class MainGuiLayerCAD : public BaseGuiLayerCAD
{
public:
	MainGuiLayerCAD(MainDataLayer& viewModel);

	virtual void DrawGui() override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
