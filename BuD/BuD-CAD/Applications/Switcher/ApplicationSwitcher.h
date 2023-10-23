#pragma once

#include <BuD.h>

#include <Applications/Switcher/DataLayers/MainDataLayerSwitcher.h>
#include <Applications/Shared/GUI/BaseGuiLayer.h>

class ApplicationSwitcher : public BuD::AppLayer
{
public:
	ApplicationSwitcher();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

	virtual void OnConcreteEvent(BuD::MouseMovedEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseScrolledEvent& e) override;

	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::KeyReleaseEvent& e) override;

protected:
	MainDataLayerSwitcher m_MainDataLayer;

	std::unique_ptr<BaseGuiLayer> m_MainGuiLayer;
};
