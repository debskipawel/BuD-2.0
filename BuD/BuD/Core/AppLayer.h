#pragma once

#include <Event/IEventDispatchable.h>

namespace BuD
{
	class AppLayer : public IEventDispatchable
	{
	public:
		virtual ~AppLayer() = default;

		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnGuiRender() = 0;
	};
}
