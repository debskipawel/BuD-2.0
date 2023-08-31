#pragma once

#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>
#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>
#include <GUI/BaseGuiLayer.h>

#include <Scene/SceneCAD.h>
#include <DataLayers/MainDataLayer.h>

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

	MainDataLayer m_MainDataLayer;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new ApplicationCAD());
}
