#pragma once

#include <BuD.h>

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>
#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

#include <DataLayers/MainDataLayer.h>
#include <GUI/BaseGuiLayer.h>

#include <Robot/Mesh/PumaMesh.h>

class PumaApp : public BuD::AppLayer
{
public:
	PumaApp();

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
	BuD::Scene m_Scene;
	MainDataLayer m_MainDataLayer;

	PumaMesh m_Mesh;

	std::unique_ptr<BaseGuiLayer> m_MainGuiLayer;
	std::unique_ptr<BaseKeyboardBehaviorLayer> m_KeyboardBehaviorLayer;
	std::unique_ptr<BaseMouseBehaviorLayer> m_MouseBehaviorLayer;
};
