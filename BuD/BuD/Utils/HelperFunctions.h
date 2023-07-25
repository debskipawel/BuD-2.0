#pragma once

#include <d3d11.h>

namespace BuD
{
    class HelperFunctions
    {
    public:
        static size_t BitsPerFormat(DXGI_FORMAT fmt);

        static size_t BytesPerFormat(DXGI_FORMAT format);
    };
}
