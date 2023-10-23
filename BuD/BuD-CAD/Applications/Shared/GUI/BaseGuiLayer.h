#pragma once

class BaseGuiLayer
{
public:
	virtual ~BaseGuiLayer() = default;

	virtual void DrawGui() = 0;
};
