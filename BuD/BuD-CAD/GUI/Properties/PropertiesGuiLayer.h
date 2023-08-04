#pragma once

#include <memory>

#include <GUI/BaseGuiLayer.h>

class PropertiesGuiLayer : public BaseGuiLayer
{
public:
	PropertiesGuiLayer();

	virtual void DrawGui() override;

protected:
	std::unique_ptr<BaseGuiLayer> m_PerformanceGuiLayer;
};
