#pragma once

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/AppStateViewModel.h>
#include <ViewModels/ObjectListViewModel.h>

class ObjectListGuiLayer : public BaseGuiLayer
{
public:
	ObjectListGuiLayer(ObjectListViewModel& objectList, AppStateViewModel& appState);

	virtual void DrawGui() override;

protected:
	virtual void DrawGuiForFilters();

	AppStateViewModel& m_AppStateViewModel;
	ObjectListViewModel& m_ObjectListViewModel;
};
