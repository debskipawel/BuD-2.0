#pragma once

#include <BuD.h>

#include <GUI/MainGuiLayer.h>

#include <Behaviors/Mouse/MainMouseBehaviorLayer.h>
#include <DataLayers/MainDataLayer.h>

class KinematicPathFinderApp : public BuD::AppLayer
{
public:
	KinematicPathFinderApp();

	virtual auto OnUpdate(float deltaTime) -> void override;
	virtual auto OnGuiRender() -> void override;
	virtual auto OnRender() -> void override;

	virtual auto OnConcreteEvent(BuD::MouseButtonDownEvent& e) -> void override;
	virtual auto OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) -> void override;
	virtual auto OnConcreteEvent(BuD::MouseMovedEvent& e) -> void override;

protected:
	MainDataLayer m_MainDataLayer;

	MainMouseBehaviorLayer m_MouseBehaviorLayer;
	MainGuiLayer m_MainGuiLayer;
};
