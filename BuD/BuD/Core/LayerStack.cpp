#include "bud_pch.h"
#include "LayerStack.h"

namespace BuD
{
	void LayerStack::PushLayer(std::shared_ptr<AppLayer> layer)
	{
		m_LayerStack.insert(m_LayerStack.begin(), layer);
	}

	void LayerStack::PopLayer(std::shared_ptr<AppLayer> layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);

		if (it == m_LayerStack.end())
		{
			return;
		}

		m_LayerStack.erase(it);
	}
}
