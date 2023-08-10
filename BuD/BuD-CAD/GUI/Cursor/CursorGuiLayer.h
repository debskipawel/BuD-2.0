#pragma once

#include <GUI/BaseGuiLayer.h>

#include <ViewModels/CursorViewModel.h>

class CursorGuiLayer : public BaseGuiLayer
{
public:
	CursorGuiLayer(CursorViewModel& cursor);

	virtual void DrawGui() override;

protected:
	CursorViewModel& m_CursorViewModel;
};
