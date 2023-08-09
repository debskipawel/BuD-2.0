#pragma once

#include <cstdint>

#include <BuD.h>

struct ViewportViewModel
{
	BuD::Texture m_ViewportImage;

	dxm::Vector2 m_ViewportPosition;

	float m_ViewportWidth;
	float m_ViewportHeight;
};
