#pragma once

#include <memory>

#include <GUI/BaseGuiLayer.h>
#include <ViewModels/AppStateViewModel.h>
#include <ViewModels/PropertiesViewModel.h>

class PropertiesGuiLayer : public BaseGuiLayer
{
public:
	PropertiesGuiLayer(PropertiesViewModel& properties, AppStateViewModel& appState);

	virtual void DrawGui() override;

protected:
	virtual void DrawGuiForSingularObject();
	virtual void DrawGuiForComposite();

	virtual bool DrawGuiForTransform(TransformComponent& transform);

	std::unique_ptr<BaseGuiLayer> m_PerformanceGuiLayer;
	
	AppStateViewModel& m_AppState;
	PropertiesViewModel& m_Properties;
};
