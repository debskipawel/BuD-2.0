#pragma once

#include <BuD.h>

#include <GUI/BaseGuiLayer.h>

#include <ViewModels/AppStateViewModel.h>
#include <ViewModels/ObjectListViewModel.h>

class AppStateGuiLayer : public BaseGuiLayer
{
public:
	AppStateGuiLayer(AppStateViewModel& appState, ObjectListViewModel& objectList);

	virtual void DrawGui() override;

protected:
	AppStateViewModel& m_AppStateViewModel;
	ObjectListViewModel& m_ObjectListViewModel;

	BuD::Texture m_IdleIcon, m_MoveIcon, m_RotateIcon, m_ScaleIcon;
};
