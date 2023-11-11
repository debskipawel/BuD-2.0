#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class CursorGuiLayer : public BaseGuiLayerCAD
{
public:
	CursorGuiLayer(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;
};
