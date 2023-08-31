#pragma once

#include <GUI/BaseGuiLayer.h>

class ViewportGuiLayer : public BaseGuiLayer
{
public:
	ViewportGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
