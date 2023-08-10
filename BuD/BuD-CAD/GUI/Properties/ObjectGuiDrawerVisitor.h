#pragma once

#include <ViewModels/PropertiesViewModel.h>
#include <Visitors/AbstractVisitor.h>

class ObjectGuiDrawerVisitor : public AbstractVisitor
{
public:
	ObjectGuiDrawerVisitor(PropertiesViewModel& viewModel);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Cube& cube) override;

protected:
	virtual bool DrawGuiForTag(SceneObjectCAD& object);
	virtual bool DrawGuiForTransform(SceneObjectCAD& object);
	virtual void DrawDeleteButton(SceneObjectCAD& object);

	PropertiesViewModel& m_ViewModel;
};
