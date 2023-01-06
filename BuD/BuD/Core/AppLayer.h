#pragma once

namespace BuD
{
	class AppLayer
	{
	public:
		virtual ~AppLayer() = default;

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnGuiRender() = 0;
	};
}
