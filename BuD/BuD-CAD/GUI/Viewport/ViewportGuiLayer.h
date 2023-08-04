#pragma once

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/ViewportViewModel.h>

class ViewportGuiLayer : public BaseGuiLayer
{
public:
	ViewportGuiLayer(ViewportViewModel& viewModel);

	virtual void DrawGui() override;

protected:
	ViewportViewModel& m_ViewModel;
};
