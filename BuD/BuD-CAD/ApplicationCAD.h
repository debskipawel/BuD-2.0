#pragma once

#include <BuD.h>
#include <Core/EntryPoint.h>

#include <GUI/MainGuiLayer.h>
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

	MainViewModel m_ViewModel;
	bool m_MoveMouse;
	bool m_InAction;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new ApplicationCAD());
}
