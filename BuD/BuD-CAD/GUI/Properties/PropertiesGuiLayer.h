#pragma once

#include <memory>

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/PropertiesViewModel.h>

class PropertiesGuiLayer : public BaseGuiLayer
{
public:
	PropertiesGuiLayer(PropertiesViewModel& viewModel);

	virtual void DrawGui() override;

protected:

	virtual void DrawGuiForSingularObject();
	virtual void DrawGuiForComposite();

	std::unique_ptr<BaseGuiLayer> m_PerformanceGuiLayer;
	PropertiesViewModel& m_ViewModel;
};
