#pragma once

#include <functional>

namespace BuD
{
	struct InstanceData
	{
	public:
		uint8_t* m_Data;
		size_t m_Size;
		size_t m_InstancesCount = 1;
	};

	struct InstanceComponent
	{
		std::function<InstanceData()> m_InstanceCallback;
	};
}
