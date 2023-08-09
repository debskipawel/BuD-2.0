#pragma once

#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Behaviors/Mouse/MouseBehaviorLayer.h>
#include <Behaviors/Keyboard/KeyboardBehaviorLayer.h>
#include <GUI/MainGuiLayer.h>
#include <Scene/SceneCAD.h>
#include <ViewModels/MainViewModel.h>

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
	std::unique_ptr<MainGuiLayer> m_GuiLayer;
	std::unique_ptr<MouseBehaviorLayer> m_MouseBehaviorLayer;
	std::unique_ptr<KeyboardBehaviorLayer> m_KeyboardBehaviorLayer;

	SceneCAD m_Scene;
	MainViewModel m_ViewModel;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new ApplicationCAD());
}
