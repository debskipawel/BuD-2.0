#pragma once

#include <BuD.h>

#include <Applications/CNC/DataLayers/MainDataLayerCNC.h>

#include <Applications/Shared/Objects/Grid.h>
#include <Applications/Shared/Behaviors/Mouse/BaseMouseBehaviorLayer.h>
#include <Applications/Shared/GUI/BaseGuiLayer.h>

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
	std::unique_ptr<BaseMouseBehaviorLayer> m_MouseBehaviorLayer;

	std::unique_ptr<Grid> m_Grid;

	MainDataLayerCNC m_MainDataLayer;
};
