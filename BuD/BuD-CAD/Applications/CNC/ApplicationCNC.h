#pragma once

#include <BuD.h>

#include <Applications/CNC/DataLayers/MainDataLayerCNC.h>
#include <GUI/BaseGuiLayer.h>

class ApplicationCNC : public BuD::AppLayer
{
public:
	ApplicationCNC();

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
	std::unique_ptr<BaseGuiLayer> m_GuiLayer;

	MainDataLayerCNC m_MainDataLayer;
};
