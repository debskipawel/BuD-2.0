#pragma once

#include <functional>

namespace BuD
{
	struct InstanceData
	{
		uint8_t* m_Data;
		size_t m_Size;
	};

	struct InstanceComponent
	{
		std::function<InstanceData()> m_InstanceCallback;
	};
}
