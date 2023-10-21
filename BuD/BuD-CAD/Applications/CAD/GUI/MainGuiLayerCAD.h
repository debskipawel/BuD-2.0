#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>

class MainGuiLayerCAD : public BaseGuiLayerCAD
{
public:
	MainGuiLayerCAD(MainDataLayerCAD& viewModel);

	virtual void DrawGui() override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
