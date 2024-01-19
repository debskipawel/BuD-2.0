#pragma once

#include <BuD.h>

#include <DataLayers/MainDataLayer.h>
#include <GUI/BaseGuiLayer.h>

class HodographApp : public BuD::AppLayer
{
public:
	HodographApp();

	virtual auto OnUpdate(float deltaTime) -> void override;

	virtual auto OnRender() -> void override;
	virtual auto OnGuiRender() -> void override;

	virtual auto OnConcreteEvent(BuD::KeyDownEvent& e) -> void override;
	virtual auto OnConcreteEvent(BuD::KeyReleaseEvent& e) -> void override;

	virtual auto OnConcreteEvent(BuD::MouseButtonDownEvent& e) -> void override;
	virtual auto OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) -> void override;
	virtual auto OnConcreteEvent(BuD::MouseScrolledEvent& e) -> void override;
	virtual auto OnConcreteEvent(BuD::MouseMovedEvent& e) -> void override;

protected:
	MainDataLayer m_MainDataLayer;

	std::unique_ptr<BaseGuiLayer> m_GuiLayer;
};
