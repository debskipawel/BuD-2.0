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

		inline std::shared_ptr<AppLayer>* begin()
		{
			auto vectorStart = (std::shared_ptr<AppLayer>*)&m_LayerStack;
			return vectorStart;
		}

		inline std::shared_ptr<AppLayer>* end()
		{
			auto vectorStart = (std::shared_ptr<AppLayer>*)&m_LayerStack;
			return vectorStart + m_LayerStack.size();
		}
	protected:
		std::vector<std::shared_ptr<AppLayer>> m_LayerStack;
	};
}
