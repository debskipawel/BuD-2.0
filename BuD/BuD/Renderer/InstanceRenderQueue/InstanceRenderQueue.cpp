#include "bud_pch.h"
#include "InstanceRenderQueue.h"

namespace BuD
{
	void InstanceRenderQueue::Add(const RenderingPass& renderingPass)
	{
		auto& instancing = renderingPass.m_Instancing.value();

		unsigned int index;
		auto instancingIndexIter = m_InstanceRenderIndices.find(renderingPass);

		if (instancingIndexIter == m_InstanceRenderIndices.end())
		{
			index = m_InstanceRenderIndices.size();

			m_InstanceRenderIndices.insert(std::make_pair(renderingPass, index));
			m_InstanceRenderingData.push_back({});
		}
		else
		{
			index = instancingIndexIter->second;
		}

		auto& rawData = m_InstanceRenderingData[index];
		auto instanceData = instancing.m_InstanceCallback();

		rawData.Append(instanceData);
	}

	void InstanceRenderQueue::Clear()
	{
		m_InstanceRenderIndices.clear();
		m_InstanceRenderingData.clear();
	}

	void InstanceRenderQueue::ForEach(std::function<void(const RenderingPass&, const InstanceRawDataBuffer&)> handle)
	{
		for (auto& [renderingPass, index] : m_InstanceRenderIndices)
		{
			auto& rawInstanceData = m_InstanceRenderingData[index];

			if (rawInstanceData.Count())
			{
				handle(renderingPass, rawInstanceData);
			}
		}
	}

	void InstanceRenderQueue::InstanceRawDataBuffer::Append(const InstanceData& instanceData)
	{
		auto currentSize = m_Data.size();
		auto newSize = currentSize + instanceData.m_Size;

		m_Data.resize(currentSize + instanceData.m_Size);

		std::memcpy(m_Data.data() + currentSize, instanceData.m_Data, instanceData.m_Size);
		m_InstanceCount += instanceData.m_InstancesCount;
	}
}
