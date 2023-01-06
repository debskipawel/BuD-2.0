#pragma once

namespace BuD
{
	class AppLayer
	{
	public:
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnGuiRender() = 0;
	};
}
