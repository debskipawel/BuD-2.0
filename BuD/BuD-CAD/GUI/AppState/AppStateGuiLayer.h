#pragma once

#include <BuD.h>

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/AppStateViewModel.h>

class AppStateGuiLayer : public BaseGuiLayer
{
public:
	AppStateGuiLayer(AppStateViewModel& viewModel);

	virtual void DrawGui() override;

protected:
	AppStateViewModel& m_ViewModel;

	BuD::Texture m_IdleIcon, m_MoveIcon, m_RotateIcon, m_ScaleIcon;
};
