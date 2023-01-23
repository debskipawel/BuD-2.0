#include "bud_pch.h"
#include "ConstantBuffer.h"

namespace BuD
{
    BufferDesc ConstantBuffer::GetBufferDesc(size_t byteWidth) const
    {
        return BufferDesc::ConstantBufferDescription(byteWidth);
    }
}
