#pragma once

#include <BuD.h>

#include <GUI/BaseGuiLayer.h>

class ViewportGuiLayer : public BaseGuiLayer
{
public:
	ViewportGuiLayer(const std::string& viewportTitle, MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	std::string m_ViewportTitle;
};
