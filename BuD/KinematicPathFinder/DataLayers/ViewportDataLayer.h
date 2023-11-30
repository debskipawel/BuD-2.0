#pragma once

#include <BuD.h>

struct ViewportDataLayer
{
	ViewportDataLayer();

	int m_Width;
	int m_Height;

	BuD::Texture m_ViewportImage;
};
