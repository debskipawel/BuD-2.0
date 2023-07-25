#pragma once

#include <Buffers/Buffer.h>

namespace BuD
{
	class InstanceBuffer : public Buffer
	{
	public:
		explicit InstanceBuffer(size_t size, const void* data = nullptr)
			: Buffer(data, GetBufferDesc(size)) {}

		virtual BufferDesc GetBufferDesc(size_t byteWidth) const override;
	};
}
