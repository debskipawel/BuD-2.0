#pragma once

#include "AppLayer.h"

#include <memory>
#include <vector>

namespace BuD
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		LayerStack(const LayerStack& other) = default;
		LayerStack& operator=(const LayerStack& other) = default;
		LayerStack& operator=(LayerStack&& other) = default;

		void PushLayer(std::shared_ptr<AppLayer>);
		void PopLayer(std::shared_ptr<AppLayer>);

		inline std::vector<std::shared_ptr<AppLayer>>::iterator begin()
		{
			return m_LayerStack.begin();
		}

		inline std::vector<std::shared_ptr<AppLayer>>::iterator end()
		{
			return m_LayerStack.end();
		}
	protected:
		std::vector<std::shared_ptr<AppLayer>> m_LayerStack;
	};
}
