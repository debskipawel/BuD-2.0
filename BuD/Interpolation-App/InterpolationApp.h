#pragma once

#include <BuD.h>

class InterpolationApp : public BuD::AppLayer
{
public:
	InterpolationApp();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

protected:
	dxm::Vector2 m_ViewportSize1, m_ViewportSize2;
	BuD::Texture m_Viewport1, m_Viewport2;

	float m_Time;
};
