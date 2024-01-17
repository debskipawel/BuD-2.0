#pragma once

#include <BuD.h>

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
};
