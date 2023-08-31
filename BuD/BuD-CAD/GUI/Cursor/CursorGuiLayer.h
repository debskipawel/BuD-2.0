#pragma once

#include <GUI/BaseGuiLayer.h>

class CursorGuiLayer : public BaseGuiLayer
{
public:
	CursorGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
