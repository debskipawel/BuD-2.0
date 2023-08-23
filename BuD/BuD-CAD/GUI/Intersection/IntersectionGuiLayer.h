#pragma once

#include <GUI/BaseGuiLayer.h>

class IntersectionGuiLayer : public BaseGuiLayer
{
public:
	explicit IntersectionGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
};
