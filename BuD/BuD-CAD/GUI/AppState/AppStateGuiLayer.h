#pragma once

#include <BuD.h>

#include <GUI/BaseGuiLayer.h>

class AppStateGuiLayer : public BaseGuiLayer
{
public:
	AppStateGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	BuD::Texture m_IdleIcon, m_MoveIcon, m_RotateIcon, m_ScaleIcon;
};
