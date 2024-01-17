#pragma once

#include <GUI/BaseGuiLayer.h>

class ViewportGuiLayer : public BaseGuiLayer
{
public:
	ViewportGuiLayer(MainDataLayer& dataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual auto DrawViewport(ViewportDataLayer& viewport, const std::string& viewportName) -> void;
};
