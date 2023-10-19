#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class CursorGuiLayer : public BaseGuiLayerCAD
{
public:
	CursorGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
