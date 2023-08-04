#pragma once

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/ObjectListViewModel.h>

class ObjectListGuiLayer : public BaseGuiLayer
{
public:
	ObjectListGuiLayer(ObjectListViewModel& viewModel);

	virtual void DrawGui() override;

protected:
	ObjectListViewModel& m_ViewModel;
};
