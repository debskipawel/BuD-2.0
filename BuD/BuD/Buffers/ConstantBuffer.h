#pragma once

#include "Buffer.h"

namespace BuD
{
	class ConstantBuffer : public Buffer
	{
	public:
		explicit ConstantBuffer(size_t size, const void* data = nullptr)
			: Buffer(data, GetBufferDesc(size)) {}

	protected:
		virtual BufferDesc GetBufferDesc(size_t byteWidth) const override;
	};
}
