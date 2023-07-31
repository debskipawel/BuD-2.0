#pragma once

#include <functional>
#include <map>
#include <vector>

#include <Buffers/InstanceBuffer.h>
#include <Renderer/GraphicsDevice.h>
#include <Renderer/RenderingPass.h>

namespace BuD
{
	class InstanceRenderQueue
	{
	public:
		void Add(const RenderingPass& renderingPass);
		void Clear();

		struct InstanceRawDataBuffer
		{
		public:
			inline const uint8_t* Data() const { return m_Data.data(); }
			inline size_t Size() const { return m_Data.size(); }
			inline unsigned int Count() const { return m_InstanceCount; }

		private:
			std::vector<uint8_t> m_Data;
			unsigned int m_InstanceCount;

			void Append(const InstanceData& instanceData);

			friend class InstanceRenderQueue;
		};

		void ForEach(std::function<void(const RenderingPass&, const InstanceRawDataBuffer&)> handle);

	protected:
		std::map<RenderingPass, unsigned int> m_InstanceRenderIndices;
		std::vector<InstanceRawDataBuffer> m_InstanceRenderingData;
	};
}
