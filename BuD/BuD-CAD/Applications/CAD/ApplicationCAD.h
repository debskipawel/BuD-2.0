#pragma once

#include <BuD.h>

#include <Applications/Shared/Behaviors/Mouse/BaseMouseBehaviorLayer.h>
#include <Applications/Shared/Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>
#include <Applications/Shared/GUI/BaseGuiLayer.h>

#include <Applications/CAD/Scene/SceneCAD.h>
#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>

class ApplicationCAD : public BuD::AppLayer
{
public:
	ApplicationCAD();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

	virtual void OnConcreteEvent(BuD::MouseMovedEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseScrolledEvent& e) override;
	
	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::KeyReleaseEvent& e) override;

private:
	std::unique_ptr<BaseGuiLayer> m_GuiLayer;
	std::unique_ptr<BaseMouseBehaviorLayer> m_MouseBehaviorLayer;
	std::unique_ptr<BaseKeyboardBehaviorLayer> m_KeyboardBehaviorLayer;

	MainDataLayerCAD m_MainDataLayer;
};
