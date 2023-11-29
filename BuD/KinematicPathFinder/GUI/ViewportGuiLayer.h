#pragma once

#include <string>

#include <imgui.h>
#include <BuD.h>

struct ViewportInfo
{
	int m_Width;
	int m_Height;
};

class ViewportGuiLayer
{
public:
	ViewportGuiLayer(const std::string& viewportTitle);

	virtual void DrawGui();
	virtual void UpdateImage(const BuD::Texture& texture);

	virtual ViewportInfo GetViewportInfo() const { return m_ViewportInfo; }

protected:
	std::string m_ViewportTitle;

	ViewportInfo m_ViewportInfo;
	BuD::Texture m_ViewportImage;
};
