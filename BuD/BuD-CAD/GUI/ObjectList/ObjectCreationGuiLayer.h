#pragma once

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/ObjectListViewModel.h>

class ObjectCreationGuiLayer : public BaseGuiLayer
{
public:
	ObjectCreationGuiLayer(ObjectListViewModel& viewModel);

	virtual void DrawGui() override;

protected:
	void CreateTorus();

	struct ButtonInfo
	{
		std::string m_Label;
		std::function<void()> m_OnClick;
	};

	std::vector<ButtonInfo> m_Buttons;

	ObjectListViewModel& m_ViewModel;
};
